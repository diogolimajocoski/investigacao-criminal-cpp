#ifndef RECURSAO_H
#define RECURSAO_H

#include "modelos.h"

// 1) Analise recursiva principal (Secao 12):
// Calcula a relevancia total a partir de uma evidencia e de toda a cadeia
// de evidencias relacionadas a ela, usando um vetor de visitados para evitar ciclos.
int calcularRelevanciaRedeRecursivo(
    const Evidencia* evidencias,
    int qtdEvidencias,
    int indiceAtual,
    bool* visitados,
    int& totalNosVisitados
);

// Soma a relevancia de todas as evidencias ligadas a uma ocorrencia (incluindo
// a rede de evidencias relacionadas de forma recursiva, sem contar duas vezes).
int analisarRelevanciaTotalOcorrencia(
    const BaseDados& base,
    int idOcorrencia,
    int& totalEvidenciasNaRede
);

// 2) Segunda funcionalidade recursiva (Secao 14):
// 2a) Conta recursivamente quantas evidencias diretas uma ocorrencia possui.
int contarEvidenciasOcorrenciaRecursivo(
    const Evidencia* evidencias,
    int qtdEvidencias,
    int idOcorrencia,
    int indiceAtual
);

// 2b) Encontra recursivamente o indice da evidencia de maior relevancia de uma ocorrencia.
// Retorna -1 caso a ocorrencia nao possua evidencias.
int encontrarMaiorRelevanciaRecursivo(
    const Evidencia* evidencias,
    int qtdEvidencias,
    int idOcorrencia,
    int indiceAtual
);

// 2c) Soma recursivamente apenas a relevancia direta das evidencias de uma ocorrencia.
int somarRelevanciaDiretaRecursivo(
    const Evidencia* evidencias,
    int qtdEvidencias,
    int idOcorrencia,
    int indiceAtual
);

// Fluxo interativo no menu principal
void analisarOcorrenciaMenu(const BaseDados& base);

#endif
