#ifndef MODELOS_H
#define MODELOS_H

// Limites maximos configuraveis em um unico ponto.
// Os vetores sao alocados dinamicamente no heap (new[] / delete[]) para suportar
// grandes volumes (ate 1.000.000 de registros) sem estourar a pilha (stack),
// respeitando a restricao de nao utilizar std::vector.
const int MAX_PESSOAS = 1000000;
const int MAX_OCORRENCIAS = 1000000;
const int MAX_EVIDENCIAS = 1000000;
const int MAX_RELACIONADAS = 10;

// Faixas de validacao
const int IDADE_MIN = 0;
const int IDADE_MAX = 130;
const int GRAVIDADE_MIN = 1; // 1 = Muito baixa
const int GRAVIDADE_MAX = 5; // 5 = Muito alta
const int ANO_MIN = 1900;
const int ANO_MAX = 2026;
const int RELEVANCIA_MIN = 1;
const int RELEVANCIA_MAX = 10;

struct Pessoa {
    int id;
    char nome[100];
    int idade;
    char cidade[50];
};

struct Ocorrencia {
    int id;
    char tipo[50];
    char local[100];
    int gravidade;
    int ano;
};

struct Evidencia {
    int id;
    int idOcorrencia;
    char descricao[100];
    int relevancia;
    // Campos adicionais (justificados no relatorio):
    // Permitem representar relacoes entre evidencias para a analise recursiva.
    int relacionadas[MAX_RELACIONADAS];
    int qtdRelacionadas;
};

enum CriterioOrdenacaoPessoa {
    PESSOA_NAO_ORDENADA = 0,
    PESSOA_ORDENADA_ID = 1,
    PESSOA_ORDENADA_NOME = 2
};

enum CriterioOrdenacaoOcorrencia {
    OCORRENCIA_NAO_ORDENADA = 0,
    OCORRENCIA_ORDENADA_ID = 1,
    OCORRENCIA_ORDENADA_GRAVIDADE = 2,
    OCORRENCIA_ORDENADA_ANO = 3,
    OCORRENCIA_ORDENADA_TIPO = 4
};

enum AlgoritmoOrdenacao {
    ALG_INSERTION_SORT = 1,
    ALG_MERGE_SORT = 2
};

struct MetricasOrdenacao {
    long long comparacoes;
    long long movimentacoes;
    double tempoMs;
};

struct BaseDados {
    Pessoa* pessoas;
    int qtdPessoas;
    CriterioOrdenacaoPessoa estadoOrdenacaoPessoas;

    Ocorrencia* ocorrencias;
    int qtdOcorrencias;
    CriterioOrdenacaoOcorrencia estadoOrdenacaoOcorrencias;

    Evidencia* evidencias;
    int qtdEvidencias;
};

// Inicializacao e liberacao da base de dados
void inicializarBase(BaseDados& base);
void liberarBase(BaseDados& base);
void limparBase(BaseDados& base);

#endif
