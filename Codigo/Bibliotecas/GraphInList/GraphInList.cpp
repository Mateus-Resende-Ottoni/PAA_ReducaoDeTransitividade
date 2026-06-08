#include "GraphInList.h"
#include <iostream>
#include <stdexcept>

GraphInList::GraphInList(int num_vertex, bool is_directed)
{
    if (num_vertex < 0)
    {
        throw std::invalid_argument("O numero de vertices nao pode ser negativo.");
    }

    this->num_vertex = num_vertex;
    this->is_directed = is_directed;
    this->vertex = new Vertex[num_vertex];

    for (int i = 0; i < num_vertex; i++)
    {
        vertex[i].setId(i);
    }
}

GraphInList::GraphInList(const GraphInList &other)
{
    this->num_vertex = other.num_vertex;
    this->is_directed = other.is_directed;
    this->vertex = new Vertex[num_vertex];

    for (int i = 0; i < num_vertex; i++)
    {
        vertex[i].setId(i);
    }

    for (int u = 0; u < num_vertex; u++)
    {
        Edge *current = other.vertex[u].getHead();
        while (current != nullptr)
        {
            this->vertex[u].addEdge(current->getIdNeighbor());
            current = current->getNext();
        }
    }
}

GraphInList::GraphInList(GraphInList &&other) noexcept
{
    this->num_vertex = other.num_vertex;
    this->is_directed = other.is_directed;
    this->vertex = other.vertex;

    other.num_vertex = 0;
    other.is_directed = true;
    other.vertex = nullptr;
}

GraphInList &GraphInList::operator=(const GraphInList &other)
{
    if (this == &other)
    {
        return *this;
    }

    delete[] this->vertex;

    this->num_vertex = other.num_vertex;
    this->is_directed = other.is_directed;
    this->vertex = new Vertex[num_vertex];

    for (int i = 0; i < num_vertex; i++)
    {
        vertex[i].setId(i);
    }

    for (int u = 0; u < num_vertex; u++)
    {
        Edge *current = other.vertex[u].getHead();
        while (current != nullptr)
        {
            this->vertex[u].addEdge(current->getIdNeighbor());
            current = current->getNext();
        }
    }

    return *this;
}

GraphInList &GraphInList::operator=(GraphInList &&other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    delete[] this->vertex;

    this->num_vertex = other.num_vertex;
    this->is_directed = other.is_directed;
    this->vertex = other.vertex;

    other.num_vertex = 0;
    other.is_directed = true;
    other.vertex = nullptr;

    return *this;
}

GraphInList::~GraphInList()
{
    delete[] vertex;
}

void GraphInList::validateVertex(int v) const
{
    if (v < 0 || v >= num_vertex)
    {
        throw std::out_of_range("Vertice fora do intervalo valido.");
    }
}

int GraphInList::getNumVertex() const
{
    return num_vertex;
}

bool GraphInList::getIsDirected() const
{
    return is_directed;
}

int GraphInList::getEdgeCount() const
{
    int count = 0;
    for (int i = 0; i < num_vertex; i++)
    {
        count += vertex[i].degree();
    }

    if (!is_directed)
    {
        count /= 2;
    }

    return count;
}

bool GraphInList::addEdge(int u, int v)
{
    validateVertex(u);
    validateVertex(v);

    bool inserted = vertex[u].addEdge(v);

    if (!is_directed)
    {
        bool reverse_inserted = vertex[v].addEdge(u);
        inserted = inserted || reverse_inserted;
    }

    return inserted;
}

bool GraphInList::removeEdge(int u, int v)
{
    validateVertex(u);
    validateVertex(v);

    bool removed = vertex[u].removeEdge(v);

    if (!is_directed)
    {
        bool reverse_removed = vertex[v].removeEdge(u);
        removed = removed || reverse_removed;
    }

    return removed;
}

bool GraphInList::hasEdge(int u, int v) const
{
    validateVertex(u);
    validateVertex(v);
    return vertex[u].hasEdge(v);
}

bool GraphInList::shouldIgnoreEdge(int u, int v, int ignored_u, int ignored_v) const
{
    if (ignored_u == -1 || ignored_v == -1)
    {
        return false;
    }

    if (u == ignored_u && v == ignored_v)
    {
        return true;
    }

    if (!is_directed && u == ignored_v && v == ignored_u)
    {
        return true;
    }

    return false;
}

bool GraphInList::dfsReachable(int current, int target, bool *visited, int ignored_u, int ignored_v) const
{
    if (current == target)
    {
        return true;
    }

    visited[current] = true;

    Edge *edge = vertex[current].getHead();
    while (edge != nullptr)
    {
        int neighbor = edge->getIdNeighbor();

        if (!shouldIgnoreEdge(current, neighbor, ignored_u, ignored_v) && !visited[neighbor])
        {
            if (dfsReachable(neighbor, target, visited, ignored_u, ignored_v))
            {
                return true;
            }
        }

        edge = edge->getNext();
    }

    return false;
}

bool GraphInList::pathExistsDFS(int source, int target, int ignored_u, int ignored_v) const
{
    validateVertex(source);
    validateVertex(target);

    if (ignored_u != -1)
    {
        validateVertex(ignored_u);
    }
    if (ignored_v != -1)
    {
        validateVertex(ignored_v);
    }

    bool *visited = new bool[num_vertex];
    for (int i = 0; i < num_vertex; i++)
    {
        visited[i] = false;
    }

    bool found = dfsReachable(source, target, visited, ignored_u, ignored_v);
    delete[] visited;
    return found;
}

GraphInList GraphInList::transitiveReductionByDFS() const
{
    if (!is_directed)
    {
        return connectivityReductionUndirectedByDFS();
    }

    GraphInList reduced(*this);

    for (int u = 0; u < num_vertex; u++)
    {
        Edge *edge = this->vertex[u].getHead();
        while (edge != nullptr)
        {
            int v = edge->getIdNeighbor();

            if (reduced.hasEdge(u, v) && reduced.pathExistsDFS(u, v, u, v))
            {
                reduced.removeEdge(u, v);
            }

            edge = edge->getNext();
        }
    }

    return reduced;
}

void GraphInList::dfsBuildUndirectedForest(int current, bool *visited, GraphInList &forest) const
{
    visited[current] = true;

    Edge *edge = vertex[current].getHead();
    while (edge != nullptr)
    {
        int neighbor = edge->getIdNeighbor();

        if (!visited[neighbor])
        {
            forest.addEdge(current, neighbor);
            dfsBuildUndirectedForest(neighbor, visited, forest);
        }

        edge = edge->getNext();
    }
}

GraphInList GraphInList::connectivityReductionUndirectedByDFS() const
{
    if (is_directed)
    {
        throw std::logic_error("A reducao por floresta geradora e propria para grafos nao direcionados.");
    }

    GraphInList forest(num_vertex, false);

    bool *visited = new bool[num_vertex];
    for (int i = 0; i < num_vertex; i++)
    {
        visited[i] = false;
    }

    for (int u = 0; u < num_vertex; u++)
    {
        if (!visited[u])
        {
            dfsBuildUndirectedForest(u, visited, forest);
        }
    }

    delete[] visited;
    return forest;
}

bool GraphInList::hasSameReachabilityAs(const GraphInList &other) const
{
    if (num_vertex != other.num_vertex || is_directed != other.is_directed)
    {
        return false;
    }

    for (int u = 0; u < num_vertex; u++)
    {
        for (int v = 0; v < num_vertex; v++)
        {
            bool this_reaches = this->pathExistsDFS(u, v);
            bool other_reaches = other.pathExistsDFS(u, v);

            if (this_reaches != other_reaches)
            {
                return false;
            }
        }
    }

    return true;
}

void GraphInList::printGraphInList() const
{
    std::cout << "Tipo: " << (is_directed ? "direcionado" : "nao direcionado") << '\n';
    std::cout << "Vertices: " << num_vertex << " | Arestas: " << getEdgeCount() << '\n';

    for (int i = 0; i < num_vertex; i++)
    {
        vertex[i].printEdges();
    }
}
