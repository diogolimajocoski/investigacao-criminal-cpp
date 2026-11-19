# Sistema de Investigacao Criminal

Projeto em C++ desenvolvido para a disciplina de **Algoritmos e Programacao de Computadores II** (Prof. Me. João Victor Duda do Amaral).

## Estrutura do Projeto

```text
sistema_investigacao/
├── .vscode/
│   └── tasks.json                  # Tasks automatizadas no VS Code (Ctrl+Shift+B)
├── include/
│   ├── modelos.h                   # Structs (Pessoa, Ocorrencia, Evidencia), limites e enums
│   ├── cadastros.h                 # Assinaturas de cadastros e validacoes
│   ├── consultas.h                 # Assinaturas de buscas (sequencial, binaria, multi-criterio)
│   ├── ordenacao.h                 # Assinaturas de Insertion Sort e Merge Sort
│   ├── recursao.h                  # Assinaturas das analises recursivas
│   └── simulacao.h                 # Gerador automatico de dados e benchmark
├── src/
│   ├── main.cpp                    # Menu principal
│   ├── cadastros.cpp               # Validacoes e insercoes seguras
│   ├── consultas.cpp               # Busca sequencial, binaria, multi-criterio e listagens
│   ├── ordenacao.cpp               # Insertion Sort e Merge Sort + metricas
│   ├── recursao.cpp                # Analise de rede de evidencias + contagem e maior relevancia
│   └── simulacao.cpp               # Gerador sintetico e relatorio de desempenho
├── tests/
│   └── testes_automatizados.cpp    # Suite de testes automatizados (validacao, busca, ordenacao, recursao)
├── build/                          # Binarios compilados (ignorado pelo Git)
├── Makefile                        # Atalhos de compilacao via terminal
└── .gitignore
```

## Como Compilar e Executar (Linux / VS Code)

### Pelo VS Code
- Pressione `Ctrl+Shift+B` para executar a task padrao **Compilar Sistema (g++)**.
- Pelo menu `Terminal -> Run Task...`, escolha:
  - **Compilar e Executar Sistema**
  - **Compilar e Rodar Testes**
  - **Limpar build**

### Pelo Terminal (g++ ou Make)
```bash
make run      # Compila e executa o sistema principal
make test     # Compila e executa os testes automatizados
make clean    # Limpa os binarios em build/
```

Ou diretamente com `g++`:
```bash
mkdir -p build
g++ -std=c++17 -O2 -Wall -Wextra -Iinclude src/main.cpp src/cadastros.cpp src/consultas.cpp src/ordenacao.cpp src/recursao.cpp src/simulacao.cpp -o build/sistema
./build/sistema
```

## Decisoes Tecnicas (Guia Rapido para Relatorio e Defesa)

### Armazenamento sem `std::vector`
Os arrays de `Pessoa`, `Ocorrencia` e `Evidencia` sao alocados dinamicamente no heap (`new[]` / `delete[]`) com capacidade configuravel em `include/modelos.h` (padrao: ate `1.000.000` registros por entidade). Isso evita estouro de pilha (`stack overflow`) e permite aumentar a escala alterando apenas constantes.

### Algoritmos de Ordenacao
- **Insertion Sort**: estavel, espaco auxiliar $O(1)$, melhor caso $O(n)$ (vetor ja ordenado), medio e pior caso $O(n^2)$.
- **Merge Sort**: estavel, dividir para conquistar, complexidade garantida $O(n \log n)$ em todos os casos, espaco auxiliar $O(n)$.

### Algoritmos de Busca
- **Busca Sequencial**: complexidade $O(n)$, funciona em qualquer ordem.
- **Busca Binaria**: complexidade $O(\log n)$, exige ordenacao previa por ID. O sistema mantem flags de estado de ordenacao e bloqueia a busca binaria quando o vetor nao esta ordenado pelo campo de busca.
- **Busca Multi-criterio (Extra)**: filtra ocorrencias por gravidade minima, ano minimo e tipo.

### Funcoes Recursivas
1. **`calcularRelevanciaRedeRecursivo`**: percorre recursivamente evidencias relacionadas (grafo de evidencias) acumulando a relevancia total da rede.
   - *Caso base*: indice invalido ou evidencia ja marcada em `visitados[]` (retorna `0`, prevenindo loops infinitos em ciclos).
   - *Caso recursivo*: marca a evidencia atual como visitada e soma sua relevancia com o retorno recursivo de cada evidencia relacionada.
2. **`contarEvidenciasOcorrenciaRecursivo`** e **`encontrarMaiorRelevanciaRecursivo`**: percorrem recursivamente o vetor de evidencias por `indiceAtual + 1` ate `qtdEvidencias` (caso base), contando evidencias diretas e identificando a de maior relevancia ($O(n)$ em tempo e pilha).
