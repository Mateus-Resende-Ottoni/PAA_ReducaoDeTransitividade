#ifndef GRAPHINMATRIX_H
#define GRAPHINMATRIX_H

#include <vector>

/// @brief
/// GraphInMatrix
/// Implementa grafos por matriz de adjacência.
///
/// A matriz usa matrix[u][v] = 1 para indicar a presença da aresta u -> v e
/// matrix[u][v] = 0 para indicar ausência. Em grafos não direcionados, a matriz
/// é mantida simétrica: inserir {u, v} marca u -> v e v -> u.
///
/// A representação por matriz tem consulta de aresta em O(1), mas consome O(V²)
/// memória.
/// Ela é especialmente natural para Warshall, pois o fecho transitivo
/// também é uma matriz booleana de alcançabilidade entre pares de vértices.
class GraphInMatrix
{
private:
    int num_vertex;
    bool is_directed;
    int **matrix;

    void allocateMatrix();
    void deallocateMatrix();

    /// @brief Valida se o vértice se encontra no intervalo admissível.    
    void validateVertex(int v) const;

    /// @brief Determina se a aresta atual é o alvo da remoção hipotética na DFS.
    bool shouldIgnoreEdge(int u, int v, int ignored_u, int ignored_v) const;

    /// @brief Núcleo da verificação de caminhos alternativos ignorando uma aresta (u, v).
    bool dfsReachable(int current, int target, bool *visited, int ignored_u, int ignored_v) const;

    /// @brief Constrói componentes conexos acíclicos para grafos não direcionados.
    void dfsBuildUndirectedForest(int current, bool *visited, GraphInMatrix &forest) const;

    /// @brief Obtém os vizinhos de saída para processamento topológico.
    std::vector<int> getOutgoingNeighbors(int u) const;

    /// @brief Verifica a existência de ciclos e gera a ordenação base (Algoritmo de Kahn).
    std::vector<int> topologicalOrderByKahn() const;

public:
    GraphInMatrix(int num_vertex, bool is_directed);
    GraphInMatrix(const GraphInMatrix &other);
    GraphInMatrix(GraphInMatrix &&other) noexcept;
    GraphInMatrix &operator=(const GraphInMatrix &other);
    GraphInMatrix &operator=(GraphInMatrix &&other) noexcept;
    ~GraphInMatrix();

    int getNumVertex() const;
    bool getIsDirected() const;

    /// @brief Retorna o total de arestas. Ajusta a contagem (divide por 2) caso o grafo não seja direcionado.
    int getEdgeCount() const;

    /// @brief Adiciona uma aresta, garantindo a invariante de um grafo simples.
    bool addEdge(int u, int v);

    bool removeEdge(int u, int v);
    bool hasEdge(int u, int v) const;

    /// @brief Verifica a existência de caminho entre origem e destino, opcionalmente ignorando uma aresta.
    bool pathExistsDFS(int source, int target, int ignored_u = -1, int ignored_v = -1) const;

    /// @brief Executa redução transitiva avaliando caminhos alternativos via DFS.
    /// @return Grafo reduzido com custo assintótico limitado a O(|E||V|^2) devido à varredura de adjacências.
    GraphInMatrix transitiveReductionByDFS() const;

    /// @brief Executa redução transitiva calculando previamente o fecho transitivo.
    /// @return Grafo reduzido em O(|V|^3). Exclusivo para grafos direcionados acíclicos (DAGs).
    GraphInMatrix transitiveReductionByWarshallForDAG() const;

    /// @brief Executa redução transitiva otimizada para DAGs.
    /// @return Grafo reduzido com custo prático mitigado pelo uso de conjuntos de 64 bits para alcançabilidade.
    GraphInMatrix transitiveReductionByReverseTopologicalOrder() const;

    /// @brief Alternativa estrutural para quando a hipótese de aciclicidade falha (grafos não direcionados).
    /// @return Floresta geradora DFS que preserva puramente a conectividade simétrica.
    GraphInMatrix connectivityReductionUndirectedByDFS() const;

    /// @brief Validação de corretude: confirma se o grafo reduzido preserva a matriz de atingibilidade original.
    bool hasSameReachabilityAs(const GraphInMatrix &other) const;
    
    void printGraphInMatrix() const;
};

#endif
