#include "cadastros.h"
#include "consultas.h"

#include <iostream>
#include <cstring>
#include <limits>
#include <cctype>

using namespace std;

void inicializarBase(BaseDados& base) {
    base.pessoas = new Pessoa[MAX_PESSOAS];
    base.qtdPessoas = 0;
    base.estadoOrdenacaoPessoas = PESSOA_NAO_ORDENADA;

    base.ocorrencias = new Ocorrencia[MAX_OCORRENCIAS];
    base.qtdOcorrencias = 0;
    base.estadoOrdenacaoOcorrencias = OCORRENCIA_NAO_ORDENADA;

    base.evidencias = new Evidencia[MAX_EVIDENCIAS];
    base.qtdEvidencias = 0;
}

void liberarBase(BaseDados& base) {
    delete[] base.pessoas;
    delete[] base.ocorrencias;
    delete[] base.evidencias;

    base.pessoas = nullptr;
    base.ocorrencias = nullptr;
    base.evidencias = nullptr;
    base.qtdPessoas = 0;
    base.qtdOcorrencias = 0;
    base.qtdEvidencias = 0;
}

void limparBase(BaseDados& base) {
    base.qtdPessoas = 0;
    base.estadoOrdenacaoPessoas = PESSOA_NAO_ORDENADA;

    base.qtdOcorrencias = 0;
    base.estadoOrdenacaoOcorrencias = OCORRENCIA_NAO_ORDENADA;

    base.qtdEvidencias = 0;
}

void limparBufferEntrada() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int lerInteiroSeguro(const char* mensagem) {
    int valor;
    while (true) {
        cout << mensagem;
        if (cin >> valor) {
            limparBufferEntrada();
            return valor;
        }
        cout << "[ERRO] Entrada invalida. Digite um numero inteiro." << endl;
        limparBufferEntrada();
    }
}

int lerInteiroNoIntervalo(const char* mensagem, int minimo, int maximo) {
    while (true) {
        int valor = lerInteiroSeguro(mensagem);
        if (valor >= minimo && valor <= maximo) {
            return valor;
        }
        cout << "[ERRO] Valor fora da faixa permitida (" << minimo << " a " << maximo << ")." << endl;
    }
}

bool textoEstaVazio(const char* texto) {
    if (texto == nullptr) {
        return true;
    }
    for (int i = 0; texto[i] != '\0'; i++) {
        if (!isspace(static_cast<unsigned char>(texto[i]))) {
            return false;
        }
    }
    return true;
}

void lerTextoObrigatorio(const char* mensagem, char* destino, int tamanhoMaximo) {
    while (true) {
        cout << mensagem;
        cin.getline(destino, tamanhoMaximo);

        if (cin.fail()) {
            limparBufferEntrada();
        }

        if (!textoEstaVazio(destino)) {
            return;
        }
        cout << "[ERRO] Este campo e obrigatorio e nao pode ficar vazio." << endl;
    }
}

bool inserirPessoa(BaseDados& base, const Pessoa& novaPessoa, char* erro, int tamErro) {
    if (base.qtdPessoas >= MAX_PESSOAS) {
        strncpy(erro, "Limite maximo de pessoas atingido.", tamErro - 1);
        erro[tamErro - 1] = '\0';
        return false;
    }
    if (novaPessoa.id <= 0) {
        strncpy(erro, "ID da pessoa deve ser maior que zero.", tamErro - 1);
        erro[tamErro - 1] = '\0';
        return false;
    }
    if (buscarPessoaSequencial(base.pessoas, base.qtdPessoas, novaPessoa.id) != -1) {
        strncpy(erro, "Ja existe uma pessoa cadastrada com este ID.", tamErro - 1);
        erro[tamErro - 1] = '\0';
        return false;
    }
    if (textoEstaVazio(novaPessoa.nome)) {
        strncpy(erro, "Nome nao pode ficar vazio.", tamErro - 1);
        erro[tamErro - 1] = '\0';
        return false;
    }
    if (novaPessoa.idade < IDADE_MIN || novaPessoa.idade > IDADE_MAX) {
        strncpy(erro, "Idade fora da faixa valida (0 a 130).", tamErro - 1);
        erro[tamErro - 1] = '\0';
        return false;
    }
    if (textoEstaVazio(novaPessoa.cidade)) {
        strncpy(erro, "Cidade nao pode ficar vazia.", tamErro - 1);
        erro[tamErro - 1] = '\0';
        return false;
    }

    base.pessoas[base.qtdPessoas] = novaPessoa;
    base.qtdPessoas++;
    base.estadoOrdenacaoPessoas = PESSOA_NAO_ORDENADA;
    return true;
}

bool inserirOcorrencia(BaseDados& base, const Ocorrencia& novaOcorrencia, char* erro, int tamErro) {
    if (base.qtdOcorrencias >= MAX_OCORRENCIAS) {
        strncpy(erro, "Limite maximo de ocorrencias atingido.", tamErro - 1);
        erro[tamErro - 1] = '\0';
        return false;
    }
    if (novaOcorrencia.id <= 0) {
        strncpy(erro, "ID da ocorrencia deve ser maior que zero.", tamErro - 1);
        erro[tamErro - 1] = '\0';
        return false;
    }
    if (buscarOcorrenciaSequencial(base.ocorrencias, base.qtdOcorrencias, novaOcorrencia.id) != -1) {
        strncpy(erro, "Ja existe uma ocorrencia cadastrada com este ID.", tamErro - 1);
        erro[tamErro - 1] = '\0';
        return false;
    }
    if (textoEstaVazio(novaOcorrencia.tipo)) {
        strncpy(erro, "Tipo da ocorrencia nao pode ficar vazio.", tamErro - 1);
        erro[tamErro - 1] = '\0';
        return false;
    }
    if (textoEstaVazio(novaOcorrencia.local)) {
        strncpy(erro, "Local da ocorrencia nao pode ficar vazio.", tamErro - 1);
        erro[tamErro - 1] = '\0';
        return false;
    }
    if (novaOcorrencia.gravidade < GRAVIDADE_MIN || novaOcorrencia.gravidade > GRAVIDADE_MAX) {
        strncpy(erro, "Gravidade invalida (use valores de 1 a 5).", tamErro - 1);
        erro[tamErro - 1] = '\0';
        return false;
    }
    if (novaOcorrencia.ano < ANO_MIN || novaOcorrencia.ano > ANO_MAX) {
        strncpy(erro, "Ano invalido (use valores de 1900 a 2026).", tamErro - 1);
        erro[tamErro - 1] = '\0';
        return false;
    }

    base.ocorrencias[base.qtdOcorrencias] = novaOcorrencia;
    base.qtdOcorrencias++;
    base.estadoOrdenacaoOcorrencias = OCORRENCIA_NAO_ORDENADA;
    return true;
}

bool inserirEvidencia(BaseDados& base, const Evidencia& novaEvidencia, char* erro, int tamErro) {
    if (base.qtdEvidencias >= MAX_EVIDENCIAS) {
        strncpy(erro, "Limite maximo de evidencias atingido.", tamErro - 1);
        erro[tamErro - 1] = '\0';
        return false;
    }
    if (novaEvidencia.id <= 0) {
        strncpy(erro, "ID da evidencia deve ser maior que zero.", tamErro - 1);
        erro[tamErro - 1] = '\0';
        return false;
    }
    if (buscarEvidenciaSequencial(base.evidencias, base.qtdEvidencias, novaEvidencia.id) != -1) {
        strncpy(erro, "Ja existe uma evidencia cadastrada com este ID.", tamErro - 1);
        erro[tamErro - 1] = '\0';
        return false;
    }
    if (buscarOcorrenciaSequencial(base.ocorrencias, base.qtdOcorrencias, novaEvidencia.idOcorrencia) == -1) {
        strncpy(erro, "Ocorrencia informada nao existe. Cadastro de evidencia impedido.", tamErro - 1);
        erro[tamErro - 1] = '\0';
        return false;
    }
    if (textoEstaVazio(novaEvidencia.descricao)) {
        strncpy(erro, "Descricao da evidencia nao pode ficar vazia.", tamErro - 1);
        erro[tamErro - 1] = '\0';
        return false;
    }
    if (novaEvidencia.relevancia < RELEVANCIA_MIN || novaEvidencia.relevancia > RELEVANCIA_MAX) {
        strncpy(erro, "Relevancia fora da faixa permitida (1 a 10).", tamErro - 1);
        erro[tamErro - 1] = '\0';
        return false;
    }

    base.evidencias[base.qtdEvidencias] = novaEvidencia;
    base.qtdEvidencias++;
    return true;
}

bool vincularEvidencias(BaseDados& base, int idOrigem, int idDestino, char* erro, int tamErro) {
    if (idOrigem == idDestino) {
        strncpy(erro, "Uma evidencia nao pode ser relacionada a ela mesma.", tamErro - 1);
        erro[tamErro - 1] = '\0';
        return false;
    }

    int idxOrigem = buscarEvidenciaSequencial(base.evidencias, base.qtdEvidencias, idOrigem);
    int idxDestino = buscarEvidenciaSequencial(base.evidencias, base.qtdEvidencias, idDestino);

    if (idxOrigem == -1 || idxDestino == -1) {
        strncpy(erro, "Evidencia de origem ou destino nao encontrada.", tamErro - 1);
        erro[tamErro - 1] = '\0';
        return false;
    }

    Evidencia& origem = base.evidencias[idxOrigem];
    if (origem.qtdRelacionadas >= MAX_RELACIONADAS) {
        strncpy(erro, "Limite maximo de relacoes atingido para esta evidencia.", tamErro - 1);
        erro[tamErro - 1] = '\0';
        return false;
    }

    for (int i = 0; i < origem.qtdRelacionadas; i++) {
        if (origem.relacionadas[i] == idDestino) {
            strncpy(erro, "Esta relacao ja foi cadastrada anteriormente.", tamErro - 1);
            erro[tamErro - 1] = '\0';
            return false;
        }
    }

    origem.relacionadas[origem.qtdRelacionadas] = idDestino;
    origem.qtdRelacionadas++;
    return true;
}

void cadastrarPessoa(BaseDados& base) {
    cout << "\n--- CADASTRAR PESSOA ---" << endl;
    if (base.qtdPessoas >= MAX_PESSOAS) {
        cout << "[ERRO] Capacidade maxima de pessoas atingida (" << MAX_PESSOAS << ")." << endl;
        return;
    }

    Pessoa p;
    p.id = lerInteiroSeguro("ID da pessoa (inteiro > 0): ");
    if (p.id <= 0) {
        cout << "[ERRO] ID deve ser positivo." << endl;
        return;
    }
    if (buscarPessoaSequencial(base.pessoas, base.qtdPessoas, p.id) != -1) {
        cout << "[ERRO] Ja existe uma pessoa com o ID " << p.id << "." << endl;
        return;
    }

    lerTextoObrigatorio("Nome completo: ", p.nome, sizeof(p.nome));
    p.idade = lerInteiroNoIntervalo("Idade (0 a 130): ", IDADE_MIN, IDADE_MAX);
    lerTextoObrigatorio("Cidade: ", p.cidade, sizeof(p.cidade));

    char erro[120];
    if (inserirPessoa(base, p, erro, sizeof(erro))) {
        cout << "[OK] Pessoa cadastrada com sucesso!" << endl;
    } else {
        cout << "[ERRO] " << erro << endl;
    }
}

void cadastrarOcorrencia(BaseDados& base) {
    cout << "\n--- CADASTRAR OCORRENCIA ---" << endl;
    if (base.qtdOcorrencias >= MAX_OCORRENCIAS) {
        cout << "[ERRO] Capacidade maxima de ocorrencias atingida (" << MAX_OCORRENCIAS << ")." << endl;
        return;
    }

    Ocorrencia o;
    o.id = lerInteiroSeguro("ID da ocorrencia (inteiro > 0): ");
    if (o.id <= 0) {
        cout << "[ERRO] ID deve ser positivo." << endl;
        return;
    }
    if (buscarOcorrenciaSequencial(base.ocorrencias, base.qtdOcorrencias, o.id) != -1) {
        cout << "[ERRO] Ja existe uma ocorrencia com o ID " << o.id << "." << endl;
        return;
    }

    lerTextoObrigatorio("Tipo (ex: Furto, Roubo, Fraude): ", o.tipo, sizeof(o.tipo));
    lerTextoObrigatorio("Local: ", o.local, sizeof(o.local));
    cout << "Niveis de gravidade: 1-Muito baixa | 2-Baixa | 3-Media | 4-Alta | 5-Muito alta" << endl;
    o.gravidade = lerInteiroNoIntervalo("Gravidade (1 a 5): ", GRAVIDADE_MIN, GRAVIDADE_MAX);
    o.ano = lerInteiroNoIntervalo("Ano (1900 a 2026): ", ANO_MIN, ANO_MAX);

    char erro[120];
    if (inserirOcorrencia(base, o, erro, sizeof(erro))) {
        cout << "[OK] Ocorrencia cadastrada com sucesso!" << endl;
    } else {
        cout << "[ERRO] " << erro << endl;
    }
}

void cadastrarEvidencia(BaseDados& base) {
    cout << "\n--- CADASTRAR EVIDENCIA ---" << endl;
    if (base.qtdEvidencias >= MAX_EVIDENCIAS) {
        cout << "[ERRO] Capacidade maxima de evidencias atingida (" << MAX_EVIDENCIAS << ")." << endl;
        return;
    }
    if (base.qtdOcorrencias == 0) {
        cout << "[AVISO] Nao ha ocorrencias cadastradas. Cadastre uma ocorrencia primeiro." << endl;
        return;
    }

    Evidencia e;
    e.qtdRelacionadas = 0;

    e.idOcorrencia = lerInteiroSeguro("Digite o ID da ocorrencia associada: ");
    int idxOc = buscarOcorrenciaSequencial(base.ocorrencias, base.qtdOcorrencias, e.idOcorrencia);
    if (idxOc == -1) {
        cout << "[ERRO] Ocorrencia " << e.idOcorrencia << " nao encontrada. Cadastro impedido." << endl;
        return;
    }

    cout << "Ocorrencia encontrada: [" << base.ocorrencias[idxOc].tipo
         << " - " << base.ocorrencias[idxOc].local << "]" << endl;

    e.id = lerInteiroSeguro("ID da evidencia (inteiro > 0): ");
    if (e.id <= 0) {
        cout << "[ERRO] ID deve ser positivo." << endl;
        return;
    }
    if (buscarEvidenciaSequencial(base.evidencias, base.qtdEvidencias, e.id) != -1) {
        cout << "[ERRO] Ja existe uma evidencia com o ID " << e.id << "." << endl;
        return;
    }

    lerTextoObrigatorio("Descricao da evidencia: ", e.descricao, sizeof(e.descricao));
    e.relevancia = lerInteiroNoIntervalo("Nivel de relevancia (1 a 10): ", RELEVANCIA_MIN, RELEVANCIA_MAX);

    char erro[120];
    if (!inserirEvidencia(base, e, erro, sizeof(erro))) {
        cout << "[ERRO] " << erro << endl;
        return;
    }

    cout << "[OK] Evidencia cadastrada com sucesso!" << endl;

    // Permite vincular evidencias ja existentes para compor a rede recursiva
    if (base.qtdEvidencias > 1) {
        int desejaVincular = lerInteiroNoIntervalo("Deseja relacionar esta evidencia a outras ja cadastradas? (1-Sim / 0-Nao): ", 0, 1);
        while (desejaVincular == 1) {
            int idRel = lerInteiroSeguro("Digite o ID da evidencia relacionada (0 para parar): ");
            if (idRel == 0) {
                break;
            }
            if (vincularEvidencias(base, e.id, idRel, erro, sizeof(erro))) {
                cout << "[OK] Relacao criada: Evidencia " << e.id << " -> Evidencia " << idRel << endl;
            } else {
                cout << "[ERRO] " << erro << endl;
            }
        }
    }
}
