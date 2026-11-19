#ifndef ORDENACAO_H
#define ORDENACAO_H

#include "modelos.h"

// Ordenacao de Pessoas (Insertion Sort e Merge Sort)
MetricasOrdenacao insertionSortPessoas(Pessoa* pessoas, int qtd, CriterioOrdenacaoPessoa criterio);
MetricasOrdenacao mergeSortPessoas(Pessoa* pessoas, int qtd, CriterioOrdenacaoPessoa criterio);

// Ordenacao de Ocorrencias (Insertion Sort e Merge Sort)
MetricasOrdenacao insertionSortOcorrencias(Ocorrencia* ocorrencias, int qtd, CriterioOrdenacaoOcorrencia criterio);
MetricasOrdenacao mergeSortOcorrencias(Ocorrencia* ocorrencias, int qtd, CriterioOrdenacaoOcorrencia criterio);

// Fluxos interativos do menu (o usuario escolhe o algoritmo e o criterio)
void ordenarPessoasMenu(BaseDados& base);
void ordenarOcorrenciasMenu(BaseDados& base);

#endif
