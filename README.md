# Sistema de Investigação Criminal

Projeto em C++ desenvolvido para a disciplina de **Algoritmos e Programação de Computadores II** (Prof. Me. João Victor Duda do Amaral).

## Estrutura do Projeto

```text
sistema_investigacao/
├── .vscode/
│   └── tasks.json                  # Tarefas automatizadas no VS Code (Ctrl+Shift+B)
├── include/
│   ├── modelos.h                   # Structs (Pessoa, Ocorrencia, Evidencia), limites e enums
│   ├── cadastros.h                 # Assinaturas de cadastros e validações
│   ├── consultas.h                 # Assinaturas de buscas (sequencial, binária, multicritério)
│   ├── ordenacao.h                 # Assinaturas de Insertion Sort e Merge Sort
│   ├── recursao.h                  # Assinaturas das análises recursivas
│   └── simulacao.h                 # Gerador automático de dados e benchmark
├── src/
│   ├── main.cpp                    # Menu principal
│   ├── cadastros.cpp               # Validações e inserções seguras
│   ├── consultas.cpp               # Busca sequencial, binária, multicritério e listagens
│   ├── ordenacao.cpp               # Insertion Sort e Merge Sort + métricas
│   ├── recursao.cpp                # Análise de rede de evidências + contagem e maior relevância
│   └── simulacao.cpp               # Gerador sintético e relatório de desempenho
├── tests/
│   └── testes_automatizados.cpp    # Suíte de testes automatizados (validação, busca, ordenação, recursão)
├── build/                          # Binários compilados (ignorado pelo Git)
├── Makefile                        # Atalhos de compilação via terminal
└── .gitignore
```

## Como Compilar e Executar (Terminal e VS Code)

### Pelo VS Code
- Pressione `Ctrl+Shift+B` para executar a tarefa padrão **Compilar Sistema (g++)**.
- Pelo menu `Terminal -> Run Task...`, escolha:
  - **Compilar e Executar Sistema**
  - **Compilar e Rodar Testes**
  - **Limpar build**

### Pelo Terminal (g++ ou Make)
```bash
make run      # Compila e executa o sistema principal
make test     # Compila e executa os testes automatizados
make clean    # Limpa os binários em build/
```

Ou diretamente com `g++`:
```bash
mkdir -p build
g++ -std=c++17 -O2 -Wall -Wextra -Iinclude src/main.cpp src/cadastros.cpp src/consultas.cpp src/ordenacao.cpp src/recursao.cpp src/simulacao.cpp -o build/sistema
./build/sistema
```

## Decisões Técnicas

### Armazenamento
Os arrays de `Pessoa`, `Ocorrencia` e `Evidencia` são alocados dinamicamente no heap (`new[]` / `delete[]`), com capacidade configurável em `include/modelos.h` (padrão: até `1.000.000` de registros por entidade). Isso evita estouro de pilha (*stack overflow*) e permite aumentar a escala alterando apenas constantes.

### Algoritmos de Ordenação
- **Insertion Sort**: estável, espaço auxiliar \(O(1)\), melhor caso \(O(n)\) (vetor já ordenado), caso médio e pior caso \(O(n^2)\).
- **Merge Sort**: estável, estratégia de divisão e conquista, complexidade garantida \(O(n \log n)\) em todos os casos, espaço auxiliar \(O(n)\).

### Algoritmos de Busca
- **Busca Sequencial**: complexidade \(O(n)\), funciona em qualquer ordem.
- **Busca Binária**: complexidade \(O(\log n)\), exige ordenação prévia por ID. O sistema mantém flags de estado de ordenação e bloqueia a busca binária quando o vetor não está ordenado pelo campo de busca.
- **Busca Multicritério (Extra)**: filtra ocorrências por gravidade mínima, ano mínimo e tipo.

### Funções Recursivas
1. **`calcularRelevanciaRedeRecursivo`**: percorre recursivamente evidências relacionadas (grafo de evidências), acumulando a relevância total da rede.
   - *Caso base*: índice inválido ou evidência já marcada em `visitados[]` (retorna `0`, prevenindo loops infinitos em ciclos).
   - *Caso recursivo*: marca a evidência atual como visitada e soma sua relevância com o retorno recursivo de cada evidência relacionada.
2. **`contarEvidenciasOcorrenciaRecursivo`** e **`encontrarMaiorRelevanciaRecursivo`**: percorrem recursivamente o vetor de evidências por `indiceAtual + 1` até `qtdEvidencias` (caso base), contando evidências diretas e identificando a de maior relevância (\(O(n)\) em tempo e espaço de pilha).
