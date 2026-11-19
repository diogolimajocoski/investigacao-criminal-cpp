#include "modelos.h"
#include "cadastros.h"
#include "consultas.h"
#include "ordenacao.h"
#include "recursao.h"
#include "simulacao.h"

#include <iostream>
#include <iomanip>
#include <cstring>
#include <cassert>

using namespace std;

static void testarCadastrosEValidacoes(BaseDados& base) {
    limparBase(base);
    char erro[120];

    // 1. Pessoa valida e ID duplicado
    Pessoa p1 = {10, "Ana Silva", 28, "Guarapuava"};
    assert(inserirPessoa(base, p1, erro, sizeof(erro)) == true);
    assert(inserirPessoa(base, p1, erro, sizeof(erro)) == false); // duplicado

    // Idade invalida e nome vazio
    Pessoa pInvalida = {11, "", 200, "Curitiba"};
    assert(inserirPessoa(base, pInvalida, erro, sizeof(erro)) == false);

    // 2. Ocorrencia valida e gravidade invalida
    Ocorrencia o1 = {152, "Furto", "Centro", 4, 2026};
    assert(inserirOcorrencia(base, o1, erro, sizeof(erro)) == true);
    assert(inserirOcorrencia(base, o1, erro, sizeof(erro)) == false); // duplicado

    Ocorrencia oGravInv = {153, "Roubo", "Batel", 9, 2026};
    assert(inserirOcorrencia(base, oGravInv, erro, sizeof(erro)) == false);

    // 3. Evidencia em ocorrencia inexistente deve falhar
    Evidencia eInvalida = {1, 999, "Sem ocorrencia", 5, {0}, 0};
    assert(inserirEvidencia(base, eInvalida, erro, sizeof(erro)) == false);

    // Evidencia valida na ocorrencia 152
    Evidencia e1 = {1, 152, "Camera de seguranca", 8, {0}, 0};
    assert(inserirEvidencia(base, e1, erro, sizeof(erro)) == true);

    cout << "[PASSOU] Testes de cadastros e validacoes." << endl;
}

static void testarOrdenacaoEBuscas(BaseDados& base) {
    gerarDadosSimulados(base, 500, 500, 1000, 77);

    // Busca sequencial funciona sem ordenacao
    assert(buscarOcorrenciaSequencial(base.ocorrencias, base.qtdOcorrencias, 250) != -1);
    assert(buscarOcorrenciaSequencial(base.ocorrencias, base.qtdOcorrencias, 999999) == -1);

    // Testa Insertion Sort em Pessoas por ID
    MetricasOrdenacao mP = insertionSortPessoas(base.pessoas, base.qtdPessoas, PESSOA_ORDENADA_ID);
    base.estadoOrdenacaoPessoas = PESSOA_ORDENADA_ID;
    for (int i = 1; i < base.qtdPessoas; i++) {
        assert(base.pessoas[i - 1].id <= base.pessoas[i].id);
    }
    assert(mP.comparacoes > 0);

    // Busca binaria em Pessoas apos ordenacao por ID
    long long compsBin = 0;
    int idxP = buscarPessoaBinaria(base.pessoas, base.qtdPessoas, 321, &compsBin);
    assert(idxP != -1 && base.pessoas[idxP].id == 321);
    assert(compsBin <= 15); // log2(500) ~ 9

    // Testa Merge Sort em Ocorrencias por Gravidade e depois por ID
    mergeSortOcorrencias(base.ocorrencias, base.qtdOcorrencias, OCORRENCIA_ORDENADA_GRAVIDADE);
    for (int i = 1; i < base.qtdOcorrencias; i++) {
        assert(base.ocorrencias[i - 1].gravidade <= base.ocorrencias[i].gravidade);
    }

    mergeSortOcorrencias(base.ocorrencias, base.qtdOcorrencias, OCORRENCIA_ORDENADA_ID);
    base.estadoOrdenacaoOcorrencias = OCORRENCIA_ORDENADA_ID;
    for (int i = 1; i < base.qtdOcorrencias; i++) {
        assert(base.ocorrencias[i - 1].id <= base.ocorrencias[i].id);
    }

    int idxO = buscarOcorrenciaBinaria(base.ocorrencias, base.qtdOcorrencias, 410);
    assert(idxO != -1 && base.ocorrencias[idxO].id == 410);

    cout << "[PASSOU] Testes de ordenacao (Insertion e Merge) e buscas (Sequencial e Binaria)." << endl;
}

static void testarRecursaoComCiclo(BaseDados& base) {
    limparBase(base);
    char erro[120];

    Ocorrencia o1 = {152, "Fraude", "Centro", 5, 2026};
    Ocorrencia o2 = {200, "Furto", "Batel", 3, 2025};
    assert(inserirOcorrencia(base, o1, erro, sizeof(erro)));
    assert(inserirOcorrencia(base, o2, erro, sizeof(erro)));

    // Evidencias da ocorrencia 152: e1(rel 6), e2(rel 9)
    // Evidencia de outra ocorrencia relacionada a e2: e3(rel 7)
    // Ciclo intencional: e1 -> e2 -> e3 -> e1 (nao pode entrar em loop infinito!)
    Evidencia e1 = {1, 152, "Planilha alterada", 6, {0}, 0};
    Evidencia e2 = {2, 152, "Transferencia bancaria", 9, {0}, 0};
    Evidencia e3 = {3, 200, "Log de acesso compartilhado", 7, {0}, 0};

    assert(inserirEvidencia(base, e1, erro, sizeof(erro)));
    assert(inserirEvidencia(base, e2, erro, sizeof(erro)));
    assert(inserirEvidencia(base, e3, erro, sizeof(erro)));

    assert(vincularEvidencias(base, 1, 2, erro, sizeof(erro)));
    assert(vincularEvidencias(base, 2, 3, erro, sizeof(erro)));
    assert(vincularEvidencias(base, 3, 1, erro, sizeof(erro))); // fecha ciclo

    int qtdDiretas = contarEvidenciasOcorrenciaRecursivo(base.evidencias, base.qtdEvidencias, 152, 0);
    assert(qtdDiretas == 2);

    int somaDireta = somarRelevanciaDiretaRecursivo(base.evidencias, base.qtdEvidencias, 152, 0);
    assert(somaDireta == 15); // 6 + 9

    int idxMaior = encontrarMaiorRelevanciaRecursivo(base.evidencias, base.qtdEvidencias, 152, 0);
    assert(idxMaior != -1 && base.evidencias[idxMaior].id == 2 && base.evidencias[idxMaior].relevancia == 9);

    int nosNaRede = 0;
    int relevanciaRede = analisarRelevanciaTotalOcorrencia(base, 152, nosNaRede);
    assert(nosNaRede == 3);
    assert(relevanciaRede == 22); // 6 + 9 + 7 (sem loop infinito!)

    cout << "[PASSOU] Testes de recursao (contagem, maior relevancia e rede com protecao contra ciclos)." << endl;
}

static void testarBenchmarkRapido() {
    cout << "\n--- BENCHMARK AUTOMATIZADO (AMOSTRA) ---" << endl;
    const int volumes[] = {1000, 5000, 10000};
    for (int i = 0; i < 3; i++) {
        int n = volumes[i];
        Ocorrencia* dados = new Ocorrencia[n];
        Ocorrencia* copia = new Ocorrencia[n];
        gerarVetorOcorrenciasTeste(dados, n, 99u + static_cast<unsigned int>(n));

        memcpy(copia, dados, sizeof(Ocorrencia) * static_cast<size_t>(n));
        MetricasOrdenacao mIns = insertionSortOcorrencias(copia, n, OCORRENCIA_ORDENADA_ID);

        memcpy(copia, dados, sizeof(Ocorrencia) * static_cast<size_t>(n));
        MetricasOrdenacao mMerge = mergeSortOcorrencias(copia, n, OCORRENCIA_ORDENADA_ID);

        cout << "N = " << setw(6) << n
             << " | Insertion: " << setw(10) << mIns.comparacoes << " comps (" << fixed << setprecision(2) << mIns.tempoMs << " ms)"
             << " | Merge: " << setw(8) << mMerge.comparacoes << " comps (" << fixed << setprecision(2) << mMerge.tempoMs << " ms)"
             << endl;

        delete[] copia;
        delete[] dados;
    }
    cout << "[PASSOU] Benchmark automatizado concluido." << endl;
}

int main() {
    BaseDados base;
    inicializarBase(base);

    cout << "============================================" << endl;
    cout << " EXECUTANDO SUITE DE TESTES AUTOMATIZADOS   " << endl;
    cout << "============================================" << endl;

    testarCadastrosEValidacoes(base);
    testarOrdenacaoEBuscas(base);
    testarRecursaoComCiclo(base);
    testarBenchmarkRapido();

    liberarBase(base);
    cout << "\nTodos os testes passaram com sucesso!" << endl;
    return 0;
}
