#ifndef GRAPHINLIST_H
#define GRAPHINLIST_H

#include "../Vertex/Vertex.h"
#include <vector>

/*
 * GraphInList
 * -----------
 * Implementa grafos por lista de adjacência.
 *
 * A representação por lista é indicada quando o grafo tende a ser esparso,
 * isto é, quando a quantidade de arestas é muito menor que V². Para o trabalho,
 * ela é útil porque permite caminhar apenas pelas arestas existentes durante
 * DFS, construção de floresta geradora e redução por ordem topológica reversa.
 *
 * O mesmo tipo representa grafos direcionados e não direcionados. Em grafos não
 * direcionados, cada aresta {u, v} é armazenada internamente duas vezes:
 * u -> v e v -> u. Por isso getEdgeCount() divide o total por 2 nesse caso.
 */
class GraphInList
{
private:
    int num_vertex;
    Vertex *vertex;
    bool is_directed;

    void validateVertex(int v) const;
    bool shouldIgnoreEdge(int u, int v, int ignored_u, int ignored_v) const;
    bool dfsReachable(int current, int target, bool *visited, int ignored_u, int ignored_v) const;
    void dfsBuildUndirectedForest(int current, bool *visited, GraphInList &forest) const;

    std::vector<int> getOutgoingNeighbors(int u) const;
    std::vector<int> topologicalOrderByKahn() const;

public:
    GraphInList(int num_vertex, bool is_directed);
    GraphInList(const GraphInList &other);
    GraphInList(GraphInList &&other) noexcept;
    GraphInList &operator=(const GraphInList &other);
    GraphInList &operator=(GraphInList &&other) noexcept;
    ~GraphInList();

    int getNumVertex() const;
    bool getIsDirected() const;
    int getEdgeCount() const;

    bool addEdge(int u, int v);
    bool removeEdge(int u, int v);
    bool hasEdge(int u, int v) const;

    bool pathExistsDFS(int source, int target, int ignored_u = -1, int ignored_v = -1) const;

    GraphInList transitiveReductionByDFS() const;
    GraphInList transitiveReductionByReverseTopologicalOrder() const;
    GraphInList connectivityReductionUndirectedByDFS() const;

    bool hasSameReachabilityAs(const GraphInList &other) const;
    void printGraphInList() const;
};

#endif
