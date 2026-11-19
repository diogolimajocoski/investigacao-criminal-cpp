#ifndef CONSULTAS_H
#define CONSULTAS_H

#include "modelos.h"

// Busca sequencial: O(n), funciona em vetores ordenados ou desordenados
int buscarPessoaSequencial(const Pessoa* pessoas, int qtd, int idAlvo, long long* comparacoes = nullptr);
int buscarOcorrenciaSequencial(const Ocorrencia* ocorrencias, int qtd, int idAlvo, long long* comparacoes = nullptr);
int buscarEvidenciaSequencial(const Evidencia* evidencias, int qtd, int idAlvo, long long* comparacoes = nullptr);

// Busca binaria: O(log n), exige vetor previamente ordenado por ID
int buscarPessoaBinaria(const Pessoa* pessoas, int qtd, int idAlvo, long long* comparacoes = nullptr);
int buscarOcorrenciaBinaria(const Ocorrencia* ocorrencias, int qtd, int idAlvo, long long* comparacoes = nullptr);

// Exibicao formatada de registros
void exibirPessoa(const Pessoa& p);
void exibirOcorrencia(const Ocorrencia& o);
void exibirEvidencia(const Evidencia& e);
const char* nomeGravidade(int gravidade);

// Fluxos interativos de consulta no menu
void consultarPessoaMenu(const BaseDados& base);
void consultarOcorrenciaMenu(const BaseDados& base);
void buscarOcorrenciasMultiCriterio(const BaseDados& base);
void listarEvidenciasDeOcorrencia(const BaseDados& base);
void exibirResumoBase(const BaseDados& base);

#endif
