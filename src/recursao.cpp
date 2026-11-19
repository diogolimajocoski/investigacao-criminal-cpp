#include "recursao.h"
#include "cadastros.h"
#include "consultas.h"

#include <iostream>

using namespace std;

// ===========================================================================
// FUNCAO RECURSIVA 1 (CENTRAL - SECAO 12):
// Analise recursiva de rede de evidencias relacionadas (DFS em grafo de evidencias)
//
// 1. Caso base:
//    indiceAtual < 0 || indiceAtual >= qtdEvidencias || visitados[indiceAtual] == true
//    -> retorna 0 (evidencia inexistente ou ja contabilizada, impedindo ciclos infinitos).
// 2. Caso recursivo:
//    Marca visitados[indiceAtual] = true, inicia soma com evidencias[indiceAtual].relevancia
//    e, para cada ID em evidencias[indiceAtual].relacionadas, localiza seu indice e
//    acumula o retorno de calcularRelevanciaRedeRecursivo(...).
// 3. Informacao passada para a proxima chamada:
//    O indice da evidencia relacionada, alem do vetor de evidencias e do vetor visitados[].
// 4. Quando termina:
//    Quando todas as evidencias alcancaveis a partir da origem foram visitadas.
// 5. Complexidade:
//    Com busca sequencial do ID relacionado: O((V + E) * n), onde V sao os nos visitados,
//    E as relacoes percorridas e n o total de evidencias cadastradas.
//    Em termos de nos visitados, cada evidencia e processada no maximo uma vez.
// ===========================================================================
int calcularRelevanciaRedeRecursivo(
    const Evidencia* evidencias,
    int qtdEvidencias,
    int indiceAtual,
    bool* visitados,
    int& totalNosVisitados
) {
    // Caso base: indice invalido ou evidencia ja visitada (evita ciclos infinitos)
    if (indiceAtual < 0 || indiceAtual >= qtdEvidencias || visitados[indiceAtual]) {
        return 0;
    }

    visitados[indiceAtual] = true;
    totalNosVisitados++;

    int somaRelevancia = evidencias[indiceAtual].relevancia;

    // Caso recursivo: percorre cada evidencia relacionada a atual
    for (int i = 0; i < evidencias[indiceAtual].qtdRelacionadas; i++) {
        int idRelacionada = evidencias[indiceAtual].relacionadas[i];
        int idxRelacionada = buscarEvidenciaSequencial(evidencias, qtdEvidencias, idRelacionada);
        if (idxRelacionada != -1 && !visitados[idxRelacionada]) {
            somaRelevancia += calcularRelevanciaRedeRecursivo(
                evidencias,
                qtdEvidencias,
                idxRelacionada,
                visitados,
                totalNosVisitados
            );
        }
    }

    return somaRelevancia;
}

int analisarRelevanciaTotalOcorrencia(
    const BaseDados& base,
    int idOcorrencia,
    int& totalEvidenciasNaRede
) {
    totalEvidenciasNaRede = 0;
    if (base.qtdEvidencias == 0) {
        return 0;
    }

    bool* visitados = new bool[base.qtdEvidencias];
    for (int i = 0; i < base.qtdEvidencias; i++) {
        visitados[i] = false;
    }

    int relevanciaTotal = 0;
    for (int i = 0; i < base.qtdEvidencias; i++) {
        if (base.evidencias[i].idOcorrencia == idOcorrencia && !visitados[i]) {
            relevanciaTotal += calcularRelevanciaRedeRecursivo(
                base.evidencias,
                base.qtdEvidencias,
                i,
                visitados,
                totalEvidenciasNaRede
            );
        }
    }

    delete[] visitados;
    return relevanciaTotal;
}

// ===========================================================================
// FUNCAO RECURSIVA 2A (SECAO 14):
// Contar recursivamente quantas evidencias diretas pertencem a uma ocorrencia
//
// 1. Caso base: indiceAtual >= qtdEvidencias -> retorna 0.
// 2. Caso recursivo: verifica se evidencias[indiceAtual].idOcorrencia == idOcorrencia
//    (contribui com 1 ou 0) e soma com a chamada para indiceAtual + 1.
// 3. Informacao passada: indiceAtual + 1 (avanca linearmente no vetor).
// 4. Quando termina: quando indiceAtual alcanca qtdEvidencias.
// 5. Complexidade: Tempo O(n), Espaco na pilha de chamadas O(n).
// ===========================================================================
int contarEvidenciasOcorrenciaRecursivo(
    const Evidencia* evidencias,
    int qtdEvidencias,
    int idOcorrencia,
    int indiceAtual
) {
    if (indiceAtual >= qtdEvidencias) {
        return 0; // Caso base
    }

    int atual = (evidencias[indiceAtual].idOcorrencia == idOcorrencia) ? 1 : 0;
    return atual + contarEvidenciasOcorrenciaRecursivo(evidencias, qtdEvidencias, idOcorrencia, indiceAtual + 1);
}

// ===========================================================================
// FUNCAO RECURSIVA 2B (SECAO 14):
// Encontrar recursivamente o indice da evidencia de maior relevancia da ocorrencia
//
// 1. Caso base: indiceAtual >= qtdEvidencias -> retorna -1 (nenhuma encontrada).
// 2. Caso recursivo: obtem o melhor indice no restante do vetor (indiceAtual + 1).
//    Se o elemento atual pertence a ocorrencia, compara sua relevancia com o melhor do resto.
// 3. Informacao passada: indiceAtual + 1.
// 4. Quando termina: quando indiceAtual alcanca qtdEvidencias.
// 5. Complexidade: Tempo O(n), Espaco na pilha de chamadas O(n).
// ===========================================================================
int encontrarMaiorRelevanciaRecursivo(
    const Evidencia* evidencias,
    int qtdEvidencias,
    int idOcorrencia,
    int indiceAtual
) {
    if (indiceAtual >= qtdEvidencias) {
        return -1; // Caso base
    }

    int melhorDoResto = encontrarMaiorRelevanciaRecursivo(evidencias, qtdEvidencias, idOcorrencia, indiceAtual + 1);

    if (evidencias[indiceAtual].idOcorrencia != idOcorrencia) {
        return melhorDoResto;
    }

    if (melhorDoResto == -1 || evidencias[indiceAtual].relevancia >= evidencias[melhorDoResto].relevancia) {
        return indiceAtual;
    }
    return melhorDoResto;
}

// ===========================================================================
// FUNCAO RECURSIVA AUXILIAR (2C):
// Somar recursivamente a relevancia direta das evidencias de uma ocorrencia
// Complexidade: Tempo O(n), Espaco O(n).
// ===========================================================================
int somarRelevanciaDiretaRecursivo(
    const Evidencia* evidencias,
    int qtdEvidencias,
    int idOcorrencia,
    int indiceAtual
) {
    if (indiceAtual >= qtdEvidencias) {
        return 0; // Caso base
    }

    int valorAtual = (evidencias[indiceAtual].idOcorrencia == idOcorrencia) ? evidencias[indiceAtual].relevancia : 0;
    return valorAtual + somarRelevanciaDiretaRecursivo(evidencias, qtdEvidencias, idOcorrencia, indiceAtual + 1);
}

// ===========================================================================
// MENU INTERATIVO DE ANALISE RECURSIVA
// ===========================================================================
void analisarOcorrenciaMenu(const BaseDados& base) {
    cout << "\n--- ANALISAR OCORRENCIA (RECURSAO) ---" << endl;
    if (base.qtdOcorrencias == 0) {
        cout << "[AVISO] Nenhuma ocorrencia cadastrada." << endl;
        return;
    }

    int idOc = lerInteiroSeguro("Digite o ID da ocorrencia para analise: ");
    int idxOc = buscarOcorrenciaSequencial(base.ocorrencias, base.qtdOcorrencias, idOc);
    if (idxOc == -1) {
        cout << "[ERRO] Ocorrencia " << idOc << " nao encontrada." << endl;
        return;
    }

    cout << "\nOcorrencia analisada:" << endl;
    exibirOcorrencia(base.ocorrencias[idxOc]);

    // Aviso de seguranca para pilha de recursao linear em bases gigantes (ex.: > 100k evidencias)
    if (base.qtdEvidencias > 100000) {
        cout << "[AVISO] A base possui " << base.qtdEvidencias
             << " evidencias. Funcoes recursivas lineares com profundidade muito alta podem exceder a pilha do sistema." << endl;
    }

    int qtdDiretas = contarEvidenciasOcorrenciaRecursivo(base.evidencias, base.qtdEvidencias, idOc, 0);
    if (qtdDiretas == 0) {
        cout << "[AVISO] Esta ocorrencia nao possui evidencias para analisar." << endl;
        return;
    }

    int relevanciaDireta = somarRelevanciaDiretaRecursivo(base.evidencias, base.qtdEvidencias, idOc, 0);
    int idxMaior = encontrarMaiorRelevanciaRecursivo(base.evidencias, base.qtdEvidencias, idOc, 0);

    int totalNaRede = 0;
    int relevanciaRede = analisarRelevanciaTotalOcorrencia(base, idOc, totalNaRede);

    cout << "\n=== RESULTADO DA ANALISE RECURSIVA ===" << endl;
    cout << "Quantidade de evidencias diretas (recursivo):        " << qtdDiretas << endl;
    cout << "Soma da relevancia direta (recursivo):               " << relevanciaDireta << endl;
    cout << "Total de evidencias na rede relacionada (recursivo): " << totalNaRede << endl;
    cout << "Relevancia total com rede de relacoes (recursivo):   " << relevanciaRede << endl;

    if (idxMaior != -1) {
        cout << "\nEvidencia de maior relevancia encontrada recursivamente:" << endl;
        exibirEvidencia(base.evidencias[idxMaior]);
    }
}
