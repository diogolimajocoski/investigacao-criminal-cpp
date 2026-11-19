#ifndef SIMULACAO_H
#define SIMULACAO_H

#include "modelos.h"

// Gera registros sinteticos validos na base ativa (sobrescreve ou limpa antes)
void gerarDadosSimulados(BaseDados& base, int qtdPessoas, int qtdOcorrencias, int qtdEvidencias, unsigned int semente = 42);

// Gera um vetor isolado de ocorrencias desordenadas para testes de desempenho
void gerarVetorOcorrenciasTeste(Ocorrencia* destino, int qtd, unsigned int semente);

// Fluxos interativos do menu
void menuGeradorSimulacao(BaseDados& base);
void executarRelatorioDesempenho();

#endif
