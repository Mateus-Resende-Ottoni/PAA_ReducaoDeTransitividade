#ifndef GRAPHINLIST_H
#define GRAPHINLIST_H

#include "../Vertex/Vertex.h"

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
    GraphInList connectivityReductionUndirectedByDFS() const;

    bool hasSameReachabilityAs(const GraphInList &other) const;
    void printGraphInList() const;
};

#endif
