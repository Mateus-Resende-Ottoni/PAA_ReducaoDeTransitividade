# Projeto e Análise de Algoritmos: Redução Transitiva em Grafos Direcionados

[![C++](https://img.shields.io/badge/Language-C++11/14/17-blue.svg)](https://isocpp.org/)
[![Status](https://img.shields.io/badge/Status-Fundação%20e%20Estruturas-orange.svg)]()
[![Disciplina](https://img.shields.io/badge/Disciplina-PAA-success.svg)]()

> **Repositório dedicado ao trabalho prático da disciplina Projeto e Análise de Algoritmos (PAA).**
> O escopo central deste projeto é o desenvolvimento, análise empírica de complexidade e implementação em C++ de um algoritmo capaz de executar a Redução Transitiva de um grafo direcionado, garantindo a eliminação de redundâncias topológicas enquanto preserva estritamente a conectividade original (atingibilidade).

---

## 📖 Índice

1. [Fundamentação Teórica](#-fundamentação-teórica)
2. [Arquitetura de Memória e Estruturas de Dados](#-arquitetura-de-memória-e-estruturas-de-dados)
3. [Estratégia Algorítmica (Em Concepção)](#-estratégia-algorítmica-em-concepção)
4. [Estrutura do Repositório](#-estrutura-do-repositório)
5. [Instruções de Compilação e Uso](#-instruções-de-compilação-e-uso)

---

## 🧠 Fundamentação Teórica

Na Teoria dos Grafos, dado um grafo direcionado $G = (V, E)$, onde $V$ é o conjunto de vértices e $E$ é o conjunto de arestas, a **atingibilidade** define se existe um caminho viável de um vértice $u$ a um vértice $v$.

A **Redução Transitiva** de $G$ é uma operação matemática que produz um subgrafo mínimo $G' = (V, E')$, onde $E' \subseteq E$. O grafo resultante $G'$ possui o menor número possível de arestas, de modo que se o vértice $v$ for alcançável a partir de $u$ no grafo original $G$, ele continuará sendo rigorosamente alcançável em $G'$.

Em termos práticos, o algoritmo identifica e extirpa "arestas de atalho". Se existe uma aresta direta $(u, v)$ e um caminho indireto simultâneo $(u \to w \to \dots \to v)$, a aresta direta $(u, v)$ é considerada transitiva (redundante) e deve ser removida da estrutura, purificando a hierarquia de dependências do grafo.

---

## ⚙️ Arquitetura de Memória e Estruturas de Dados

O diferencial técnico desta fase inicial da implementação (presente em `Codigo/Grafos.cpp`) é a recusa ao uso de bibliotecas de abstração prontas (como o `std::vector` da STL). Todo o gerenciamento de memória dinâmica (_Heap_) e o mapeamento de ponteiros foram construídos do zero ("na mão"), visando o controle absoluto sobre o desempenho e a alocação.

Para suportar uma avaliação científica rigorosa entre diferentes densidades de grafos, o sistema implementa duas abstrações estruturais completas:

### 1. `GraphInList` (Lista de Adjacência)

Projetada meticulosamente para **grafos esparsos** (onde $|E| \ll |V|^2$).

- **Estrutura:** O grafo contém um _array_ dinâmico de objetos `Vertex`. Cada `Vertex` gerencia uma lista encadeada singular controlando seus vizinhos via ponteiros de cabeça (`head`) compostos por objetos `Edge`.
- **Complexidade de Inserção:** $\mathcal{O}(1)$ absoluto, utilizando técnica de inserção no início da lista.
- **Vantagem:** Ocupa espaço linear $\mathcal{O}(V + E)$ e permite caminhamentos de exploração (DFS/BFS) altamente otimizados.

### 2. `GraphInMatrix` (Matriz de Adjacência)

Projetada estritamente para **grafos densos** (onde $|E| \approx |V|^2$).

- **Estrutura:** Implementada via ponteiros duplos (`int**`), alocando bidimensionalmente uma matriz dinâmica de tamanho $V \times V$.
- **Complexidade de Inserção e Consulta:** $\mathcal{O}(1)$ para acesso ou modificação direta da relação de incidência.
- **Vantagem e Flexibilidade:** Incorpora nativamente um _flag_ de estado (`is_directed`) que, se configurado como falso, garante matematicamente a simetria da matriz algébrica espelhando automaticamente as atualizações para as células opostas.

---

## 💡 Estratégia Algorítmica (Em Concepção)

A fundação do algoritmo de redução transitiva (atualmente documentada como protótipo analítico em `transitividade.txt`) avalia inicialmente uma heurística baseada na união de conjuntos:

1. **Ordenação Topológica:** Um pré-processamento que hierarquiza os nós, assegurando que o caminhamento flua respeitando a cascata de dependências do grafo.
2. **Avaliação de Pertinência de Grupo:** O rascunho lógico sugere injetar sucessores nos "grupos de atingibilidade" dos antecessores.
3. **Limitação Conhecida (O Paradoxo da Distância):** O documento textualmente evidencia um caso de borda mapeado: a falha em lidar com nós conectados localizados a uma distância $>1$ nos cenários onde uma aresta redundante cria uma ponte de salto longo.

**Próximos Passos de Arquitetura:** A mitigação desse paradigma transitará para a implementação orgânica de uma **Busca em Profundidade (DFS) com oclusão seletiva** utilizando as recém-criadas classes de base (`GraphInList`), garantindo uma varredura sistêmica de completude para cada aresta candidata a remoção.

---

## 📂 Estrutura do Repositório

O projeto segue boas práticas de organização, segregando a arquitetura base (classes genéricas de estruturas de dados) da lógica específica de resolução analítica:

```text
PAA_ReducaoDeTransitividade/
│
├── Codigo/
│   └── Grafos.cpp           # Classes fundamentais (Edge, Vertex, GraphInList, GraphInMatrix)
│
├── transitividade.txt       # Arquivo de log lógico, mapeamento de falhas e planejamento
│
└── README.md                # Este documento de referência arquitetural
```

---

## 🚀 Instruções de Compilação e Uso

Os arquivos-fonte foram escritos em C++ padrão, operando sob gestão manual rigorosa de ponteiros e destrutores (memory leak free).

##### Processo de Compilação (Linux/macOS)

No terminal, certifique-se de estar dentro do diretório Codigo/ e utilize o compilador de sua preferência (ex: GCC)

```
g++ -std=c++11 -Wall -O3 Grafos.cpp -o estruturas_teste
```

Nota: A flag -O3 habilita otimizações agressivas em tempo de compilação.

Execução de Teste da Infraestrutura
Para invocar o instanciamento e validar o funcionamento das classes-base sem vazamentos de memória:

```
./estruturas_teste
```
⚠️ (Nota: Certifique-se de inserir uma função `int main()` ao final de **Grafos.cpp** antes da compilação para injetar as arestas desejadas nos objetos e realizar a impressão dos grafos via métodos nativos).

