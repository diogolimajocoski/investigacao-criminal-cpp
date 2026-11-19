#include "modelos.h"
#include "cadastros.h"
#include "consultas.h"
#include "ordenacao.h"
#include "recursao.h"
#include "simulacao.h"

#include <iostream>

using namespace std;

static void exibirMenuPrincipal() {
    cout << "\n========================================" << endl;
    cout << "        SISTEMA DE INVESTIGACAO         " << endl;
    cout << "========================================" << endl;
    cout << "1  - Cadastrar pessoa" << endl;
    cout << "2  - Cadastrar ocorrencia" << endl;
    cout << "3  - Cadastrar evidencia" << endl;
    cout << "4  - Buscar pessoa" << endl;
    cout << "5  - Buscar ocorrencia" << endl;
    cout << "6  - Ordenar pessoas" << endl;
    cout << "7  - Ordenar ocorrencias" << endl;
    cout << "8  - Listar evidencias de uma ocorrencia" << endl;
    cout << "9  - Analisar ocorrencia (recursao)" << endl;
    cout << "10 - Relatorio de desempenho" << endl;
    cout << "11 - Gerar dados de simulacao" << endl;
    cout << "12 - Resumo da base de dados" << endl;
    cout << "0  - Encerrar programa" << endl;
    cout << "========================================" << endl;
}

int main() {
    BaseDados base;
    inicializarBase(base);

    int opcao = -1;
    do {
        exibirMenuPrincipal();
        opcao = lerInteiroNoIntervalo("Digite uma opcao: ", 0, 12);

        switch (opcao) {
            case 1:  cadastrarPessoa(base); break;
            case 2:  cadastrarOcorrencia(base); break;
            case 3:  cadastrarEvidencia(base); break;
            case 4:  consultarPessoaMenu(base); break;
            case 5:  consultarOcorrenciaMenu(base); break;
            case 6:  ordenarPessoasMenu(base); break;
            case 7:  ordenarOcorrenciasMenu(base); break;
            case 8:  listarEvidenciasDeOcorrencia(base); break;
            case 9:  analisarOcorrenciaMenu(base); break;
            case 10: executarRelatorioDesempenho(); break;
            case 11: menuGeradorSimulacao(base); break;
            case 12: exibirResumoBase(base); break;
            case 0:  cout << "Encerrando sistema..." << endl; break;
            default: cout << "[ERRO] Opcao invalida." << endl; break;
        }
    } while (opcao != 0);

    liberarBase(base);
    return 0;
}
