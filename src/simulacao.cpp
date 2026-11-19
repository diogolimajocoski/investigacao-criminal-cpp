#include "simulacao.h"
#include "cadastros.h"
#include "ordenacao.h"

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstring>

using namespace std;

// Gerador congruencial linear simples e deterministico (rapido e portavel)
static unsigned int proximoAleatorio(unsigned int& estado) {
    estado = estado * 1664525u + 1013904223u;
    return estado;
}

static const char* NOMES_BASE[] = {
    "Ana Silva", "Bruno Costa", "Carla Mendes", "Daniel Oliveira",
    "Eduarda Lima", "Felipe Rocha", "Gabriela Alves", "Henrique Dias",
    "Isabela Martins", "Joao Ferreira", "Larissa Gomes", "Marcos Pereira"
};
static const int QTD_NOMES_BASE = 12;

static const char* CIDADES_BASE[] = {
    "Guarapuava", "Curitiba", "Londrina", "Maringa",
    "Ponta Grossa", "Cascavel", "Foz do Iguacu", "Toledo"
};
static const int QTD_CIDADES_BASE = 8;

static const char* TIPOS_CRIME[] = {
    "Furto", "Roubo", "Fraude", "Estelionato",
    "Dano ao Patrimonio", "Vandalismo", "Ameaca", "Invasao"
};
static const int QTD_TIPOS_CRIME = 8;

static const char* LOCAIS_BASE[] = {
    "Centro", "Bairro Batel", "Bairro Santa Cruz", "Zona Industrial",
    "Rodovia BR-277", "Praca Central", "Terminal Urbano", "Bairro Bonsucesso"
};
static const int QTD_LOCAIS_BASE = 8;

static const char* DESCRICOES_EVIDENCIA[] = {
    "Imagem de camera de seguranca",
    "Impressao digital coletada no local",
    "Depoimento de testemunha ocular",
    "Registro telefonico suspeito",
    "Nota fiscal encontrada na cena",
    "Fragmento de objeto apreendido",
    "Log de acesso eletronico",
    "Laudo pericial preliminar"
};
static const int QTD_DESCRICOES = 8;

// Embaralhamento Fisher-Yates para garantir IDs unicos em ordem aleatoria
static void embaralharIds(int* ids, int qtd, unsigned int& estado) {
    for (int i = 0; i < qtd; i++) {
        ids[i] = i + 1;
    }
    for (int i = qtd - 1; i > 0; i--) {
        int j = static_cast<int>(proximoAleatorio(estado) % static_cast<unsigned int>(i + 1));
        int tmp = ids[i];
        ids[i] = ids[j];
        ids[j] = tmp;
    }
}

void gerarVetorOcorrenciasTeste(Ocorrencia* destino, int qtd, unsigned int semente) {
    unsigned int estado = semente;
    int* ids = new int[qtd];
    embaralharIds(ids, qtd, estado);

    for (int i = 0; i < qtd; i++) {
        destino[i].id = ids[i];
        const char* tipo = TIPOS_CRIME[proximoAleatorio(estado) % QTD_TIPOS_CRIME];
        const char* local = LOCAIS_BASE[proximoAleatorio(estado) % QTD_LOCAIS_BASE];
        strncpy(destino[i].tipo, tipo, sizeof(destino[i].tipo) - 1);
        destino[i].tipo[sizeof(destino[i].tipo) - 1] = '\0';

        snprintf(destino[i].local, sizeof(destino[i].local), "%s - Setor %d", local, (i % 50) + 1);
        destino[i].gravidade = static_cast<int>(proximoAleatorio(estado) % 5) + 1;
        destino[i].ano = 2015 + static_cast<int>(proximoAleatorio(estado) % 12); // 2015 a 2026
    }

    delete[] ids;
}

void gerarDadosSimulados(BaseDados& base, int qtdPessoas, int qtdOcorrencias, int qtdEvidencias, unsigned int semente) {
    if (qtdPessoas > MAX_PESSOAS) qtdPessoas = MAX_PESSOAS;
    if (qtdOcorrencias > MAX_OCORRENCIAS) qtdOcorrencias = MAX_OCORRENCIAS;
    if (qtdEvidencias > MAX_EVIDENCIAS) qtdEvidencias = MAX_EVIDENCIAS;

    limparBase(base);
    unsigned int estado = semente;

    // 1. Gerar pessoas com IDs unicos embaralhados
    if (qtdPessoas > 0) {
        int* idsP = new int[qtdPessoas];
        embaralharIds(idsP, qtdPessoas, estado);
        for (int i = 0; i < qtdPessoas; i++) {
            Pessoa& p = base.pessoas[i];
            p.id = idsP[i];
            const char* nomeBase = NOMES_BASE[proximoAleatorio(estado) % QTD_NOMES_BASE];
            snprintf(p.nome, sizeof(p.nome), "%s #%d", nomeBase, p.id);
            p.idade = 18 + static_cast<int>(proximoAleatorio(estado) % 63); // 18 a 80
            const char* cidade = CIDADES_BASE[proximoAleatorio(estado) % QTD_CIDADES_BASE];
            strncpy(p.cidade, cidade, sizeof(p.cidade) - 1);
            p.cidade[sizeof(p.cidade) - 1] = '\0';
        }
        delete[] idsP;
        base.qtdPessoas = qtdPessoas;
    }

    // 2. Gerar ocorrencias com IDs unicos embaralhados
    if (qtdOcorrencias > 0) {
        gerarVetorOcorrenciasTeste(base.ocorrencias, qtdOcorrencias, semente + 7);
        base.qtdOcorrencias = qtdOcorrencias;
    }

    // 3. Gerar evidencias associadas a ocorrencias existentes + rede de relacoes
    if (qtdEvidencias > 0 && qtdOcorrencias > 0) {
        for (int i = 0; i < qtdEvidencias; i++) {
            Evidencia& e = base.evidencias[i];
            e.id = i + 1;
            int idxOc = static_cast<int>(proximoAleatorio(estado) % static_cast<unsigned int>(qtdOcorrencias));
            e.idOcorrencia = base.ocorrencias[idxOc].id;

            const char* desc = DESCRICOES_EVIDENCIA[proximoAleatorio(estado) % QTD_DESCRICOES];
            snprintf(e.descricao, sizeof(e.descricao), "%s (Lote %d)", desc, e.id);
            e.relevancia = static_cast<int>(proximoAleatorio(estado) % 10) + 1;
            e.qtdRelacionadas = 0;

            // Cria 1 ou 2 relacoes com evidencias anteriores para formar cadeias/grafos testaveis
            if (i > 0 && (proximoAleatorio(estado) % 100) < 45) {
                int idRel1 = static_cast<int>(proximoAleatorio(estado) % static_cast<unsigned int>(i)) + 1;
                e.relacionadas[e.qtdRelacionadas++] = idRel1;

                if (i > 2 && (proximoAleatorio(estado) % 100) < 25) {
                    int idRel2 = static_cast<int>(proximoAleatorio(estado) % static_cast<unsigned int>(i)) + 1;
                    if (idRel2 != idRel1) {
                        e.relacionadas[e.qtdRelacionadas++] = idRel2;
                    }
                }
            }
        }
        base.qtdEvidencias = qtdEvidencias;
    }

    base.estadoOrdenacaoPessoas = PESSOA_NAO_ORDENADA;
    base.estadoOrdenacaoOcorrencias = OCORRENCIA_NAO_ORDENADA;
}

void menuGeradorSimulacao(BaseDados& base) {
    cout << "\n--- GERADOR DE DADOS PARA SIMULACAO ---" << endl;
    cout << "ATENCAO: Esta operacao substitui os registros atualmente carregados na memoria." << endl;
    cout << "1 - Carga pequena  (100 pessoas, 50 ocorrencias, 150 evidencias) -> ideal para testar recursao e menu" << endl;
    cout << "2 - Carga minima do trabalho (10.000 pessoas, 5.000 ocorrencias, 20.000 evidencias)" << endl;
    cout << "3 - Carga grande   (50.000 pessoas, 50.000 ocorrencias, 50.000 evidencias)" << endl;
    cout << "4 - Carga personalizada" << endl;
    cout << "0 - Cancelar" << endl;

    int op = lerInteiroNoIntervalo("Escolha uma opcao: ", 0, 4);
    if (op == 0) return;

    int np = 0, no = 0, ne = 0;
    if (op == 1) {
        np = 100; no = 50; ne = 150;
    } else if (op == 2) {
        np = 10000; no = 5000; ne = 20000;
    } else if (op == 3) {
        np = 50000; no = 50000; ne = 50000;
    } else {
        np = lerInteiroNoIntervalo("Quantidade de pessoas (0 a 1000000): ", 0, MAX_PESSOAS);
        no = lerInteiroNoIntervalo("Quantidade de ocorrencias (0 a 1000000): ", 0, MAX_OCORRENCIAS);
        ne = lerInteiroNoIntervalo("Quantidade de evidencias (0 a 1000000): ", 0, MAX_EVIDENCIAS);
    }

    gerarDadosSimulados(base, np, no, ne, 42);
    cout << "[OK] Simulacao gerada com sucesso!" << endl;
    cout << "Pessoas: " << base.qtdPessoas
         << " | Ocorrencias: " << base.qtdOcorrencias
         << " | Evidencias: " << base.qtdEvidencias << endl;
}

static void imprimirLinhaTabela(const char* algoritmo, int registros, const MetricasOrdenacao& m) {
    cout << left << setw(16) << algoritmo
         << " | " << right << setw(10) << registros
         << " | " << right << setw(15) << m.comparacoes
         << " | " << right << setw(15) << m.movimentacoes
         << " | " << right << setw(10) << fixed << setprecision(3) << m.tempoMs << " ms"
         << endl;
}

void executarRelatorioDesempenho() {
    cout << "\n--- RELATORIO DE DESEMPENHO (COMPARACAO AUTOMATICA) ---" << endl;
    cout << "1 - Padrao obrigatorio (1.000, 5.000, 10.000 e 50.000 registros)" << endl;
    cout << "2 - Estresse extremo   (inclui 100.000, 500.000 e 1.000.000 no Merge Sort)" << endl;
    cout << "0 - Voltar" << endl;

    int modo = lerInteiroNoIntervalo("Escolha o modo do benchmark: ", 0, 2);
    if (modo == 0) return;

    const int volumesPadrao[] = {1000, 5000, 10000, 50000};
    const int volumesExtremos[] = {1000, 5000, 10000, 50000, 100000, 500000, 1000000};

    const int* volumes = (modo == 1) ? volumesPadrao : volumesExtremos;
    int qtdVolumes = (modo == 1) ? 4 : 7;

    cout << "\nTestando ordenacao de Ocorrencias por ID (mesmos dados desordenados para ambos):" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << left << setw(16) << "Algoritmo"
         << " | " << right << setw(10) << "Registros"
         << " | " << right << setw(15) << "Comparacoes"
         << " | " << right << setw(15) << "Movimentacoes"
         << " | " << right << setw(13) << "Tempo" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;

    for (int v = 0; v < qtdVolumes; v++) {
        int n = volumes[v];

        Ocorrencia* baseTeste = new Ocorrencia[n];
        Ocorrencia* copia = new Ocorrencia[n];
        gerarVetorOcorrenciasTeste(baseTeste, n, 12345u + static_cast<unsigned int>(n));

        // Insertion Sort ate 50.000 (acima disso O(n^2) leva minutos em structs de ~164 bytes)
        if (n <= 50000) {
            memcpy(copia, baseTeste, sizeof(Ocorrencia) * static_cast<size_t>(n));
            MetricasOrdenacao mIns = insertionSortOcorrencias(copia, n, OCORRENCIA_ORDENADA_ID);
            imprimirLinhaTabela("Insertion Sort", n, mIns);
        } else {
            cout << left << setw(16) << "Insertion Sort"
                 << " | " << right << setw(10) << n
                 << " | " << right << setw(15) << "(omitido O(n^2))"
                 << " | " << right << setw(15) << "-"
                 << " | " << right << setw(13) << "muito lento" << endl;
        }

        // Merge Sort em todos os volumes (inclusive 1.000.000)
        memcpy(copia, baseTeste, sizeof(Ocorrencia) * static_cast<size_t>(n));
        MetricasOrdenacao mMerge = mergeSortOcorrencias(copia, n, OCORRENCIA_ORDENADA_ID);
        imprimirLinhaTabela("Merge Sort", n, mMerge);

        cout << "--------------------------------------------------------------------------------" << endl;

        delete[] copia;
        delete[] baseTeste;
    }

    cout << "\nConclusao pratica: enquanto o Insertion Sort cresce quadraticamente O(n^2)," << endl;
    cout << "o Merge Sort mantem crescimento O(n log n), viabilizando grandes volumes." << endl;
}
