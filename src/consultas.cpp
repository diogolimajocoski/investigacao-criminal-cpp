#include "consultas.h"
#include "cadastros.h"

#include <iostream>
#include <cstring>
#include <cctype>

using namespace std;

static bool textosIguaisSemCase(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (tolower(static_cast<unsigned char>(a[i])) != tolower(static_cast<unsigned char>(b[i]))) {
            return false;
        }
        i++;
    }
    return a[i] == '\0' && b[i] == '\0';
}

// ===========================================================================
// ALGORITMOS DE BUSCA (implementacao manual - sem std::binary_search)
// ===========================================================================

// Busca Sequencial em Pessoas - Complexidade: O(n)
int buscarPessoaSequencial(const Pessoa* pessoas, int qtd, int idAlvo, long long* comparacoes) {
    long long comps = 0;
    for (int i = 0; i < qtd; i++) {
        comps++;
        if (pessoas[i].id == idAlvo) {
            if (comparacoes != nullptr) *comparacoes = comps;
            return i;
        }
    }
    if (comparacoes != nullptr) *comparacoes = comps;
    return -1;
}

// Busca Sequencial em Ocorrencias - Complexidade: O(n)
int buscarOcorrenciaSequencial(const Ocorrencia* ocorrencias, int qtd, int idAlvo, long long* comparacoes) {
    long long comps = 0;
    for (int i = 0; i < qtd; i++) {
        comps++;
        if (ocorrencias[i].id == idAlvo) {
            if (comparacoes != nullptr) *comparacoes = comps;
            return i;
        }
    }
    if (comparacoes != nullptr) *comparacoes = comps;
    return -1;
}

// Busca Sequencial em Evidencias - Complexidade: O(n)
int buscarEvidenciaSequencial(const Evidencia* evidencias, int qtd, int idAlvo, long long* comparacoes) {
    long long comps = 0;
    for (int i = 0; i < qtd; i++) {
        comps++;
        if (evidencias[i].id == idAlvo) {
            if (comparacoes != nullptr) *comparacoes = comps;
            return i;
        }
    }
    if (comparacoes != nullptr) *comparacoes = comps;
    return -1;
}

// Busca Binaria em Pessoas (exige ordenacao por ID) - Complexidade: O(log n)
int buscarPessoaBinaria(const Pessoa* pessoas, int qtd, int idAlvo, long long* comparacoes) {
    int inicio = 0;
    int fim = qtd - 1;
    long long comps = 0;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        comps++;
        if (pessoas[meio].id == idAlvo) {
            if (comparacoes != nullptr) *comparacoes = comps;
            return meio;
        }
        if (pessoas[meio].id < idAlvo) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    if (comparacoes != nullptr) *comparacoes = comps;
    return -1;
}

// Busca Binaria em Ocorrencias (exige ordenacao por ID) - Complexidade: O(log n)
int buscarOcorrenciaBinaria(const Ocorrencia* ocorrencias, int qtd, int idAlvo, long long* comparacoes) {
    int inicio = 0;
    int fim = qtd - 1;
    long long comps = 0;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        comps++;
        if (ocorrencias[meio].id == idAlvo) {
            if (comparacoes != nullptr) *comparacoes = comps;
            return meio;
        }
        if (ocorrencias[meio].id < idAlvo) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    if (comparacoes != nullptr) *comparacoes = comps;
    return -1;
}

// ===========================================================================
// EXIBICAO DE REGISTROS
// ===========================================================================

const char* nomeGravidade(int gravidade) {
    switch (gravidade) {
        case 1: return "Muito baixa";
        case 2: return "Baixa";
        case 3: return "Media";
        case 4: return "Alta";
        case 5: return "Muito alta";
        default: return "Desconhecida";
    }
}

void exibirPessoa(const Pessoa& p) {
    cout << "ID: " << p.id
         << " | Nome: " << p.nome
         << " | Idade: " << p.idade
         << " | Cidade: " << p.cidade << endl;
}

void exibirOcorrencia(const Ocorrencia& o) {
    cout << "ID: " << o.id
         << " | Tipo: " << o.tipo
         << " | Local: " << o.local
         << " | Gravidade: " << o.gravidade << " (" << nomeGravidade(o.gravidade) << ")"
         << " | Ano: " << o.ano << endl;
}

void exibirEvidencia(const Evidencia& e) {
    cout << "ID: " << e.id
         << " | Ocorrencia: " << e.idOcorrencia
         << " | Relevancia: " << e.relevancia
         << " | Descricao: " << e.descricao;
    if (e.qtdRelacionadas > 0) {
        cout << " | Relacionadas: [";
        for (int i = 0; i < e.qtdRelacionadas; i++) {
            cout << e.relacionadas[i];
            if (i + 1 < e.qtdRelacionadas) cout << ", ";
        }
        cout << "]";
    }
    cout << endl;
}

// ===========================================================================
// MENUS INTERATIVOS DE CONSULTA
// ===========================================================================

void consultarPessoaMenu(const BaseDados& base) {
    cout << "\n--- BUSCAR PESSOA ---" << endl;
    if (base.qtdPessoas == 0) {
        cout << "[AVISO] Nenhuma pessoa cadastrada no sistema." << endl;
        return;
    }

    cout << "1 - Busca sequencial por ID (funciona sempre)" << endl;
    cout << "2 - Busca binaria por ID (exige ordenacao previa por ID)" << endl;
    int metodo = lerInteiroNoIntervalo("Escolha o metodo de busca: ", 1, 2);

    if (metodo == 2 && base.estadoOrdenacaoPessoas != PESSOA_ORDENADA_ID) {
        cout << "[ERRO] Busca binaria bloqueada: o vetor de pessoas nao esta ordenado por ID." << endl;
        cout << "Ordene as pessoas por ID na opcao 6 do menu antes de usar busca binaria." << endl;
        return;
    }

    int idAlvo = lerInteiroSeguro("Digite o ID da pessoa: ");
    long long comparacoes = 0;
    int idx = (metodo == 1)
        ? buscarPessoaSequencial(base.pessoas, base.qtdPessoas, idAlvo, &comparacoes)
        : buscarPessoaBinaria(base.pessoas, base.qtdPessoas, idAlvo, &comparacoes);

    if (idx != -1) {
        cout << "\nPessoa encontrada!" << endl;
        exibirPessoa(base.pessoas[idx]);
    } else {
        cout << "\nPessoa com ID " << idAlvo << " nao encontrada." << endl;
    }
    cout << "Comparacoes realizadas: " << comparacoes << endl;
}

void buscarOcorrenciasMultiCriterio(const BaseDados& base) {
    cout << "\n--- BUSCA DE OCORRENCIAS POR MULTIPLOS CRITERIOS ---" << endl;
    if (base.qtdOcorrencias == 0) {
        cout << "[AVISO] Nenhuma ocorrencia cadastrada no sistema." << endl;
        return;
    }

    int gravMin = lerInteiroNoIntervalo("Gravidade minima (1 a 5): ", GRAVIDADE_MIN, GRAVIDADE_MAX);
    int anoMin = lerInteiroNoIntervalo("Ano minimo (1900 a 2026): ", ANO_MIN, ANO_MAX);
    char tipoFiltro[50];
    cout << "Tipo exato (deixe vazio + ENTER para ignorar filtro de tipo): ";
    cin.getline(tipoFiltro, sizeof(tipoFiltro));
    if (cin.fail()) {
        limparBufferEntrada();
    }
    bool filtrarTipo = !textoEstaVazio(tipoFiltro);

    int encontradas = 0;
    const int LIMITE_IMPRESSAO = 20;

    for (int i = 0; i < base.qtdOcorrencias; i++) {
        const Ocorrencia& o = base.ocorrencias[i];
        bool okGrav = (o.gravidade >= gravMin);
        bool okAno = (o.ano >= anoMin);
        bool okTipo = (!filtrarTipo) || textosIguaisSemCase(o.tipo, tipoFiltro);

        if (okGrav && okAno && okTipo) {
            encontradas++;
            if (encontradas <= LIMITE_IMPRESSAO) {
                exibirOcorrencia(o);
            }
        }
    }

    if (encontradas == 0) {
        cout << "Nenhuma ocorrencia atendeu aos criterios informados." << endl;
    } else {
        if (encontradas > LIMITE_IMPRESSAO) {
            cout << "... exibindo os primeiros " << LIMITE_IMPRESSAO << " resultados." << endl;
        }
        cout << "Total de ocorrencias encontradas: " << encontradas << endl;
    }
}

void consultarOcorrenciaMenu(const BaseDados& base) {
    cout << "\n--- BUSCAR OCORRENCIA ---" << endl;
    if (base.qtdOcorrencias == 0) {
        cout << "[AVISO] Nenhuma ocorrencia cadastrada no sistema." << endl;
        return;
    }

    cout << "1 - Busca sequencial por ID (funciona sempre)" << endl;
    cout << "2 - Busca binaria por ID (exige ordenacao previa por ID)" << endl;
    cout << "3 - Busca por multiplos criterios (gravidade >= X, ano >= Y, tipo)" << endl;
    int metodo = lerInteiroNoIntervalo("Escolha o metodo de busca: ", 1, 3);

    if (metodo == 3) {
        buscarOcorrenciasMultiCriterio(base);
        return;
    }

    if (metodo == 2 && base.estadoOrdenacaoOcorrencias != OCORRENCIA_ORDENADA_ID) {
        cout << "[ERRO] Busca binaria bloqueada: o vetor de ocorrencias nao esta ordenado por ID." << endl;
        cout << "Ordene as ocorrencias por ID na opcao 7 do menu antes de usar busca binaria." << endl;
        return;
    }

    int idAlvo = lerInteiroSeguro("Digite o ID da ocorrencia: ");
    long long comparacoes = 0;
    int idx = (metodo == 1)
        ? buscarOcorrenciaSequencial(base.ocorrencias, base.qtdOcorrencias, idAlvo, &comparacoes)
        : buscarOcorrenciaBinaria(base.ocorrencias, base.qtdOcorrencias, idAlvo, &comparacoes);

    if (idx != -1) {
        cout << "\nOcorrencia encontrada!" << endl;
        exibirOcorrencia(base.ocorrencias[idx]);
    } else {
        cout << "\nOcorrencia com ID " << idAlvo << " nao encontrada." << endl;
    }
    cout << "Comparacoes realizadas: " << comparacoes << endl;
}

void listarEvidenciasDeOcorrencia(const BaseDados& base) {
    cout << "\n--- LISTAR EVIDENCIAS DE UMA OCORRENCIA ---" << endl;
    if (base.qtdOcorrencias == 0) {
        cout << "[AVISO] Nenhuma ocorrencia cadastrada." << endl;
        return;
    }

    int idOc = lerInteiroSeguro("Digite o ID da ocorrencia: ");
    int idxOc = buscarOcorrenciaSequencial(base.ocorrencias, base.qtdOcorrencias, idOc);
    if (idxOc == -1) {
        cout << "[ERRO] Ocorrencia " << idOc << " nao encontrada." << endl;
        return;
    }

    cout << "\nOcorrencia selecionada:" << endl;
    exibirOcorrencia(base.ocorrencias[idxOc]);
    cout << "Evidencias associadas:" << endl;

    int total = 0;
    for (int i = 0; i < base.qtdEvidencias; i++) {
        if (base.evidencias[i].idOcorrencia == idOc) {
            exibirEvidencia(base.evidencias[i]);
            total++;
        }
    }

    if (total == 0) {
        cout << "[AVISO] Esta ocorrencia nao possui evidencias cadastradas." << endl;
    } else {
        cout << "Total de evidencias listadas: " << total << endl;
    }
}

void exibirResumoBase(const BaseDados& base) {
    cout << "\n--- RESUMO DA BASE DE DADOS ---" << endl;
    cout << "Pessoas cadastradas:     " << base.qtdPessoas << " / " << MAX_PESSOAS << endl;
    cout << "Ocorrencias cadastradas: " << base.qtdOcorrencias << " / " << MAX_OCORRENCIAS << endl;
    cout << "Evidencias cadastradas:  " << base.qtdEvidencias << " / " << MAX_EVIDENCIAS << endl;

    cout << "Estado ordenacao Pessoas:     ";
    switch (base.estadoOrdenacaoPessoas) {
        case PESSOA_ORDENADA_ID:   cout << "Ordenada por ID" << endl; break;
        case PESSOA_ORDENADA_NOME: cout << "Ordenada por Nome" << endl; break;
        default:                   cout << "Nao ordenada" << endl; break;
    }

    cout << "Estado ordenacao Ocorrencias: ";
    switch (base.estadoOrdenacaoOcorrencias) {
        case OCORRENCIA_ORDENADA_ID:        cout << "Ordenada por ID" << endl; break;
        case OCORRENCIA_ORDENADA_GRAVIDADE: cout << "Ordenada por Gravidade" << endl; break;
        case OCORRENCIA_ORDENADA_ANO:       cout << "Ordenada por Ano" << endl; break;
        case OCORRENCIA_ORDENADA_TIPO:      cout << "Ordenada por Tipo" << endl; break;
        default:                            cout << "Nao ordenada" << endl; break;
    }

    int amostra = (base.qtdOcorrencias < 5) ? base.qtdOcorrencias : 5;
    if (amostra > 0) {
        cout << "\nPrimeiras " << amostra << " ocorrencias:" << endl;
        for (int i = 0; i < amostra; i++) {
            exibirOcorrencia(base.ocorrencias[i]);
        }
    }
}
