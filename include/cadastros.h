#ifndef CADASTROS_H
#define CADASTROS_H

#include "modelos.h"

// Utilitarios de entrada segura (evita loops infinitos com entradas invalidas)
void limparBufferEntrada();
int lerInteiroSeguro(const char* mensagem);
int lerInteiroNoIntervalo(const char* mensagem, int minimo, int maximo);
void lerTextoObrigatorio(const char* mensagem, char* destino, int tamanhoMaximo);
bool textoEstaVazio(const char* texto);

// Funcoes de insercao programatica (reutilizadas pelo menu, simulador e testes)
bool inserirPessoa(BaseDados& base, const Pessoa& novaPessoa, char* erro, int tamErro);
bool inserirOcorrencia(BaseDados& base, const Ocorrencia& novaOcorrencia, char* erro, int tamErro);
bool inserirEvidencia(BaseDados& base, const Evidencia& novaEvidencia, char* erro, int tamErro);
bool vincularEvidencias(BaseDados& base, int idOrigem, int idDestino, char* erro, int tamErro);

// Funcoes interativas chamadas pelo menu principal
void cadastrarPessoa(BaseDados& base);
void cadastrarOcorrencia(BaseDados& base);
void cadastrarEvidencia(BaseDados& base);

#endif
