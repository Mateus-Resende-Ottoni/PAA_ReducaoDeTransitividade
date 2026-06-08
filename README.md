# PAA - Redução de Transitividade em Grafos

Este projeto implementa, em C++, estruturas básicas de grafos e algoritmos para o trabalho de Projeto e Análise de Algoritmos.

## O que foi implementado

- Grafo direcionado por lista de adjacência (`GraphInList`).
- Grafo direcionado por matriz de adjacência (`GraphInMatrix`).
- Grafo não direcionado nas duas representações.
- Inserção, remoção e consulta de arestas.
- Busca em profundidade para verificar atingibilidade.
- Redução transitiva segura por DFS para grafos direcionados.
- Redução por fecho transitivo/Warshall para DAGs, usando matriz de adjacência.
- Redução de conectividade para grafos não direcionados, por floresta geradora DFS.
- Validação simples de preservação de atingibilidade.
- Execução com exemplos internos ou com arquivo de entrada.

## Compilação

Na raiz do projeto:

```bash
make
```

Ou manualmente:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -O2 \
  Codigo/Grafos.cpp \
  Codigo/Bibliotecas/Edge/Edge.cpp \
  Codigo/Bibliotecas/Vertex/Vertex.cpp \
  Codigo/Bibliotecas/GraphInList/GraphInList.cpp \
  Codigo/Bibliotecas/GraphInMatrix/GraphInMatrix.cpp \
  -o paa_transitividade
```

## Execução

### Rodar exemplos internos

```bash
./paa_transitividade
```

### Rodar a partir de arquivo

```bash
./paa_transitividade exemplos/direcionado.txt
./paa_transitividade exemplos/nao_direcionado.txt
```

Formato do arquivo:

```text
n m direcionado
u1 v1
u2 v2
...
```

Onde:

- `n` = número de vértices.
- `m` = número de arestas.
- `direcionado` = `1` para grafo direcionado e `0` para grafo não direcionado.
- Cada linha seguinte contém uma aresta `u v`.

## Observação conceitual

A redução transitiva, é uma operação naturalmente definida para grafos direcionados, especialmente DAGs. Para grafos não direcionados, a operação análoga implementada aqui é uma redução de conectividade: remove ciclos e mantém uma floresta geradora, preservando quais vértices são alcançáveis entre si.
