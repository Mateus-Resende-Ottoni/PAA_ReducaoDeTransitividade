#ifndef GRAPHINMATRIX_H
#define GRAPHINMATRIX_H

#include <vector>

/*
 * GraphInMatrix
 * -------------
 * Implementa grafos por matriz de adjacência.
 *
 * A matriz usa matrix[u][v] = 1 para indicar a presença da aresta u -> v e
 * matrix[u][v] = 0 para indicar ausência. Em grafos não direcionados, a matriz
 * é mantida simétrica: inserir {u, v} marca u -> v e v -> u.
 *
 * A representação por matriz tem consulta de aresta em O(1), mas consome O(V²)
 * memória. Ela é especialmente natural para Warshall, pois o fecho transitivo
 * também é uma matriz booleana de alcançabilidade entre pares de vértices.
 */
class GraphInMatrix
{
private:
    int num_vertex;
    bool is_directed;
    int **matrix;

    void allocateMatrix();
    void deallocateMatrix();
    void validateVertex(int v) const;
    bool shouldIgnoreEdge(int u, int v, int ignored_u, int ignored_v) const;
    bool dfsReachable(int current, int target, bool *visited, int ignored_u, int ignored_v) const;
    void dfsBuildUndirectedForest(int current, bool *visited, GraphInMatrix &forest) const;

    std::vector<int> getOutgoingNeighbors(int u) const;
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
    int getEdgeCount() const;

    bool addEdge(int u, int v);
    bool removeEdge(int u, int v);
    bool hasEdge(int u, int v) const;

    bool pathExistsDFS(int source, int target, int ignored_u = -1, int ignored_v = -1) const;

    GraphInMatrix transitiveReductionByDFS() const;
    GraphInMatrix transitiveReductionByWarshallForDAG() const;
    GraphInMatrix transitiveReductionByReverseTopologicalOrder() const;
    GraphInMatrix connectivityReductionUndirectedByDFS() const;

    bool hasSameReachabilityAs(const GraphInMatrix &other) const;
    void printGraphInMatrix() const;
};

#endif
