# PAA - Redução de Transitividade em Grafos

Este projeto implementa, em C++, estruturas básicas de grafos e diferentes estratégias para o trabalho de Projeto e Análise de Algoritmos.

O foco principal é a **remoção de arestas redundantes preservando a atingibilidade** entre os vértices. Em grafos direcionados acíclicos, isso corresponde à redução transitiva clássica. Em grafos não direcionados, a noção foi tratada separadamente, pois a ideia de transitividade é naturalmente orientada por direção de caminho.

## O que foi implementado

- Grafo direcionado por lista de adjacência (`GraphInList`).
- Grafo direcionado por matriz de adjacência (`GraphInMatrix`).
- Grafo não direcionado nas duas representações.
- Inserção, remoção e consulta de arestas.
- Busca em profundidade para verificar atingibilidade.
- Redução transitiva segura por DFS para grafos direcionados.
- Redução por fecho transitivo/Warshall para DAGs, usando matriz de adjacência.
- Redução transitiva por **ordenação topológica reversa**, implementada em lista e matriz.
- Remoção de ciclos para grafos não direcionados, por floresta geradora DFS (operação equivalente).
- Validação simples de preservação de atingibilidade.
- Tratamento mais robusto para entradas inválidas, laços, ciclos em algoritmos que exigem DAG e cabeçalhos de arquivo incorretos.
- Comentários explicativos no código, priorizando documentação de decisões algorítmicas e responsabilidades das classes.

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

### Rodar bateria de testes
```bash
./paa_transitividade --test
```
Resultados são guardados no arquivo "resultados_paa.csv",  na pasta "Resultados" na raiz do projeto

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

### Gerar graficos
Requer a instalação de pandas, matplotlib, seaborn em python 

```bash
pip install pandas matplotlib seaborn
```

Para gerar, basta usar o comando, na pasta raiz, num ambient python (env)

```bash
python plotar_resultados.py
```
As imagens resultantes chamadas "grafico_denso.png" e "grafico_esparso" estarão na pasta "Resultados".

## Abordagens de redução implementadas

### 1. Redução transitiva por DFS

Para cada aresta `u -> v`, o algoritmo verifica se ainda existe caminho de `u` até `v` ignorando essa própria aresta. Se existir, a aresta direta é redundante e pode ser removida sem alterar a atingibilidade.

Essa abordagem é simples, direta e funciona como uma interpretação operacional do enunciado. Ela tende a ser mais cara porque pode executar uma DFS para muitas arestas.

### 2. Redução por Warshall / fecho transitivo

Na matriz de adjacência, o algoritmo de Warshall calcula o fecho transitivo do grafo. Depois, uma aresta `u -> v` é considerada redundante quando existe outro sucessor direto `w` de `u` tal que `w` alcança `v`.

Esta abordagem foi restringida a DAGs, pois a redução transitiva clássica é única em grafos direcionados acíclicos. Em grafos direcionados com ciclos, o problema passa a exigir escolhas adicionais, como tratamento por componentes fortemente conexos.

### 3. Redução por ordenação topológica reversa

Essa é a terceira abordagem adicionada ao projeto.

O algoritmo primeiro calcula uma ordenação topológica usando o algoritmo de Kahn. Em seguida, percorre os vértices de trás para frente. Como o grafo é um DAG, todos os sucessores de um vértice `u` aparecem depois de `u` na ordenação topológica; logo, ao processar `u` em ordem reversa, a atingibilidade dos sucessores já está conhecida.

O algoritmo mantém um conjunto de bits `reachable[u]` indicando quais vértices são alcançáveis a partir de `u`. A aresta `u -> v` é removida se existir outro sucessor direto `w` de `u` que já alcance `v`, pois o caminho `u -> w ... v` substitui a aresta direta.

### 4. Redução de transitividade em grafos não direcionados

Em grafos não direcionados, a operação equivalente não é redução transitiva no mesmo sentido de DAGs. A solução implementada remove ciclos e preserva componentes conexos, produzindo uma árvore geradora quando o grafo é conexo ou uma floresta geradora quando há múltiplos componentes.

## Observação conceitual

A redução transitiva é naturalmente definida para grafos direcionados, especialmente DAGs. Para grafos não direcionados, a operação análoga implementada aqui tem uma natureza diferente: remove se ciclos e mantém uma floresta geradora, preservando quais vértices são alcançáveis entre si.
