#include "ordenacao.h"
#include "cadastros.h"
#include "consultas.h"

#include <iostream>
#include <cstring>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

// ===========================================================================
// COMPARADORES POR CRITERIO (retornam true se 'a' deve vir antes ou igual a 'b')
// ===========================================================================

static bool pessoaMenorOuIgual(const Pessoa& a, const Pessoa& b, CriterioOrdenacaoPessoa criterio) {
    switch (criterio) {
        case PESSOA_ORDENADA_ID:
            return a.id <= b.id;
        case PESSOA_ORDENADA_NOME: {
            int cmp = strcmp(a.nome, b.nome);
            if (cmp != 0) return cmp < 0;
            return a.id <= b.id; // desempate deterministico por ID
        }
        default:
            return a.id <= b.id;
    }
}

static bool pessoaMaiorQue(const Pessoa& a, const Pessoa& b, CriterioOrdenacaoPessoa criterio) {
    return !pessoaMenorOuIgual(a, b, criterio);
}

static bool ocorrenciaMenorOuIgual(const Ocorrencia& a, const Ocorrencia& b, CriterioOrdenacaoOcorrencia criterio) {
    switch (criterio) {
        case OCORRENCIA_ORDENADA_ID:
            return a.id <= b.id;
        case OCORRENCIA_ORDENADA_GRAVIDADE:
            if (a.gravidade != b.gravidade) return a.gravidade < b.gravidade;
            return a.id <= b.id;
        case OCORRENCIA_ORDENADA_ANO:
            if (a.ano != b.ano) return a.ano < b.ano;
            return a.id <= b.id;
        case OCORRENCIA_ORDENADA_TIPO: {
            int cmp = strcmp(a.tipo, b.tipo);
            if (cmp != 0) return cmp < 0;
            return a.id <= b.id;
        }
        default:
            return a.id <= b.id;
    }
}

static bool ocorrenciaMaiorQue(const Ocorrencia& a, const Ocorrencia& b, CriterioOrdenacaoOcorrencia criterio) {
    return !ocorrenciaMenorOuIgual(a, b, criterio);
}

// ===========================================================================
// INSERTION SORT (implementacao manual)
// Complexidade: Melhor caso O(n), Caso medio e Pior caso O(n^2)
// Espaco auxiliar: O(1) | Estavel: Sim
// ===========================================================================

MetricasOrdenacao insertionSortPessoas(Pessoa* pessoas, int qtd, CriterioOrdenacaoPessoa criterio) {
    MetricasOrdenacao m = {0, 0, 0.0};
    auto inicio = steady_clock::now();

    for (int i = 1; i < qtd; i++) {
        Pessoa chave = pessoas[i];
        m.movimentacoes++;
        int j = i - 1;

        while (j >= 0) {
            m.comparacoes++;
            if (pessoaMaiorQue(pessoas[j], chave, criterio)) {
                pessoas[j + 1] = pessoas[j];
                m.movimentacoes++;
                j--;
            } else {
                break;
            }
        }
        pessoas[j + 1] = chave;
        m.movimentacoes++;
    }

    auto fim = steady_clock::now();
    m.tempoMs = duration<double, milli>(fim - inicio).count();
    return m;
}

MetricasOrdenacao insertionSortOcorrencias(Ocorrencia* ocorrencias, int qtd, CriterioOrdenacaoOcorrencia criterio) {
    MetricasOrdenacao m = {0, 0, 0.0};
    auto inicio = steady_clock::now();

    for (int i = 1; i < qtd; i++) {
        Ocorrencia chave = ocorrencias[i];
        m.movimentacoes++;
        int j = i - 1;

        while (j >= 0) {
            m.comparacoes++;
            if (ocorrenciaMaiorQue(ocorrencias[j], chave, criterio)) {
                ocorrencias[j + 1] = ocorrencias[j];
                m.movimentacoes++;
                j--;
            } else {
                break;
            }
        }
        ocorrencias[j + 1] = chave;
        m.movimentacoes++;
    }

    auto fim = steady_clock::now();
    m.tempoMs = duration<double, milli>(fim - inicio).count();
    return m;
}

// ===========================================================================
// MERGE SORT (implementacao manual - dividir para conquistar)
// Complexidade: Melhor, Medio e Pior caso O(n log n)
// Espaco auxiliar: O(n) | Estavel: Sim
// ===========================================================================

static void intercalarPessoas(
    Pessoa* vetor,
    Pessoa* auxiliar,
    int inicio,
    int meio,
    int fim,
    CriterioOrdenacaoPessoa criterio,
    MetricasOrdenacao& m
) {
    int i = inicio;
    int j = meio + 1;
    int k = inicio;

    while (i <= meio && j <= fim) {
        m.comparacoes++;
        if (pessoaMenorOuIgual(vetor[i], vetor[j], criterio)) {
            auxiliar[k++] = vetor[i++];
        } else {
            auxiliar[k++] = vetor[j++];
        }
        m.movimentacoes++;
    }

    while (i <= meio) {
        auxiliar[k++] = vetor[i++];
        m.movimentacoes++;
    }

    while (j <= fim) {
        auxiliar[k++] = vetor[j++];
        m.movimentacoes++;
    }

    for (int idx = inicio; idx <= fim; idx++) {
        vetor[idx] = auxiliar[idx];
        m.movimentacoes++;
    }
}

static void mergeSortPessoasRec(
    Pessoa* vetor,
    Pessoa* auxiliar,
    int inicio,
    int fim,
    CriterioOrdenacaoPessoa criterio,
    MetricasOrdenacao& m
) {
    if (inicio >= fim) {
        return; // Caso base: subvetor com 0 ou 1 elemento ja esta ordenado
    }
    int meio = inicio + (fim - inicio) / 2;
    mergeSortPessoasRec(vetor, auxiliar, inicio, meio, criterio, m);
    mergeSortPessoasRec(vetor, auxiliar, meio + 1, fim, criterio, m);
    intercalarPessoas(vetor, auxiliar, inicio, meio, fim, criterio, m);
}

MetricasOrdenacao mergeSortPessoas(Pessoa* pessoas, int qtd, CriterioOrdenacaoPessoa criterio) {
    MetricasOrdenacao m = {0, 0, 0.0};
    if (qtd <= 1) {
        return m;
    }

    Pessoa* auxiliar = new Pessoa[qtd];
    auto inicio = steady_clock::now();
    mergeSortPessoasRec(pessoas, auxiliar, 0, qtd - 1, criterio, m);
    auto fim = steady_clock::now();
    delete[] auxiliar;

    m.tempoMs = duration<double, milli>(fim - inicio).count();
    return m;
}

static void intercalarOcorrencias(
    Ocorrencia* vetor,
    Ocorrencia* auxiliar,
    int inicio,
    int meio,
    int fim,
    CriterioOrdenacaoOcorrencia criterio,
    MetricasOrdenacao& m
) {
    int i = inicio;
    int j = meio + 1;
    int k = inicio;

    while (i <= meio && j <= fim) {
        m.comparacoes++;
        if (ocorrenciaMenorOuIgual(vetor[i], vetor[j], criterio)) {
            auxiliar[k++] = vetor[i++];
        } else {
            auxiliar[k++] = vetor[j++];
        }
        m.movimentacoes++;
    }

    while (i <= meio) {
        auxiliar[k++] = vetor[i++];
        m.movimentacoes++;
    }

    while (j <= fim) {
        auxiliar[k++] = vetor[j++];
        m.movimentacoes++;
    }

    for (int idx = inicio; idx <= fim; idx++) {
        vetor[idx] = auxiliar[idx];
        m.movimentacoes++;
    }
}

static void mergeSortOcorrenciasRec(
    Ocorrencia* vetor,
    Ocorrencia* auxiliar,
    int inicio,
    int fim,
    CriterioOrdenacaoOcorrencia criterio,
    MetricasOrdenacao& m
) {
    if (inicio >= fim) {
        return; // Caso base: subvetor unitario
    }
    int meio = inicio + (fim - inicio) / 2;
    mergeSortOcorrenciasRec(vetor, auxiliar, inicio, meio, criterio, m);
    mergeSortOcorrenciasRec(vetor, auxiliar, meio + 1, fim, criterio, m);
    intercalarOcorrencias(vetor, auxiliar, inicio, meio, fim, criterio, m);
}

MetricasOrdenacao mergeSortOcorrencias(Ocorrencia* ocorrencias, int qtd, CriterioOrdenacaoOcorrencia criterio) {
    MetricasOrdenacao m = {0, 0, 0.0};
    if (qtd <= 1) {
        return m;
    }

    Ocorrencia* auxiliar = new Ocorrencia[qtd];
    auto inicio = steady_clock::now();
    mergeSortOcorrenciasRec(ocorrencias, auxiliar, 0, qtd - 1, criterio, m);
    auto fim = steady_clock::now();
    delete[] auxiliar;

    m.tempoMs = duration<double, milli>(fim - inicio).count();
    return m;
}

// ===========================================================================
// MENUS INTERATIVOS DE ORDENACAO
// ===========================================================================

void ordenarPessoasMenu(BaseDados& base) {
    cout << "\n--- ORDENAR PESSOAS ---" << endl;
    if (base.qtdPessoas == 0) {
        cout << "[AVISO] Nenhuma pessoa cadastrada." << endl;
        return;
    }

    cout << "Escolha o algoritmo:" << endl;
    cout << "1 - Insertion Sort  [O(n^2)]" << endl;
    cout << "2 - Merge Sort      [O(n log n)]" << endl;
    int alg = lerInteiroNoIntervalo("Algoritmo: ", 1, 2);

    cout << "Escolha o criterio de ordenacao:" << endl;
    cout << "1 - Ordenar por ID" << endl;
    cout << "2 - Ordenar por Nome" << endl;
    int critInt = lerInteiroNoIntervalo("Criterio: ", 1, 2);
    CriterioOrdenacaoPessoa crit = static_cast<CriterioOrdenacaoPessoa>(critInt);

    MetricasOrdenacao m;
    if (alg == ALG_INSERTION_SORT) {
        if (base.qtdPessoas > 50000) {
            cout << "[AVISO] Insertion Sort em " << base.qtdPessoas << " registros pode demorar bastante." << endl;
        }
        m = insertionSortPessoas(base.pessoas, base.qtdPessoas, crit);
    } else {
        m = mergeSortPessoas(base.pessoas, base.qtdPessoas, crit);
    }

    base.estadoOrdenacaoPessoas = crit;

    cout << "\n[OK] Pessoas ordenadas com sucesso!" << endl;
    cout << "Registros:     " << base.qtdPessoas << endl;
    cout << "Comparacoes:   " << m.comparacoes << endl;
    cout << "Movimentacoes: " << m.movimentacoes << endl;
    cout << fixed << setprecision(3) << "Tempo (ms):    " << m.tempoMs << endl;

    int amostra = (base.qtdPessoas < 5) ? base.qtdPessoas : 5;
    cout << "\nPrimeiros " << amostra << " registros apos ordenacao:" << endl;
    for (int i = 0; i < amostra; i++) {
        exibirPessoa(base.pessoas[i]);
    }
}

void ordenarOcorrenciasMenu(BaseDados& base) {
    cout << "\n--- ORDENAR OCORRENCIAS ---" << endl;
    if (base.qtdOcorrencias == 0) {
        cout << "[AVISO] Nenhuma ocorrencia cadastrada." << endl;
        return;
    }

    cout << "Escolha o algoritmo:" << endl;
    cout << "1 - Insertion Sort  [O(n^2)]" << endl;
    cout << "2 - Merge Sort      [O(n log n)]" << endl;
    int alg = lerInteiroNoIntervalo("Algoritmo: ", 1, 2);

    cout << "Escolha o criterio de ordenacao:" << endl;
    cout << "1 - Ordenar por ID" << endl;
    cout << "2 - Ordenar por Gravidade" << endl;
    cout << "3 - Ordenar por Ano" << endl;
    cout << "4 - Ordenar por Tipo" << endl;
    int critInt = lerInteiroNoIntervalo("Criterio: ", 1, 4);
    CriterioOrdenacaoOcorrencia crit = static_cast<CriterioOrdenacaoOcorrencia>(critInt);

    MetricasOrdenacao m;
    if (alg == ALG_INSERTION_SORT) {
        if (base.qtdOcorrencias > 50000) {
            cout << "[AVISO] Insertion Sort em " << base.qtdOcorrencias << " registros pode demorar bastante." << endl;
        }
        m = insertionSortOcorrencias(base.ocorrencias, base.qtdOcorrencias, crit);
    } else {
        m = mergeSortOcorrencias(base.ocorrencias, base.qtdOcorrencias, crit);
    }

    base.estadoOrdenacaoOcorrencias = crit;

    cout << "\n[OK] Ocorrencias ordenadas com sucesso!" << endl;
    cout << "Registros:     " << base.qtdOcorrencias << endl;
    cout << "Comparacoes:   " << m.comparacoes << endl;
    cout << "Movimentacoes: " << m.movimentacoes << endl;
    cout << fixed << setprecision(3) << "Tempo (ms):    " << m.tempoMs << endl;

    int amostra = (base.qtdOcorrencias < 5) ? base.qtdOcorrencias : 5;
    cout << "\nPrimeiras " << amostra << " ocorrencias apos ordenacao:" << endl;
    for (int i = 0; i < amostra; i++) {
        exibirOcorrencia(base.ocorrencias[i]);
    }
}
