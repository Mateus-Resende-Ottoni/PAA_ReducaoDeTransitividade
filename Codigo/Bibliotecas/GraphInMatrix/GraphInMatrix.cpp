#include "GraphInMatrix.h"
#include <iostream>
#include <stdexcept>

GraphInMatrix::GraphInMatrix(int num_vertex, bool is_directed)
{
    if (num_vertex < 0)
    {
        throw std::invalid_argument("O numero de vertices nao pode ser negativo.");
    }

    this->num_vertex = num_vertex;
    this->is_directed = is_directed;
    this->matrix = nullptr;
    allocateMatrix();
}

GraphInMatrix::GraphInMatrix(const GraphInMatrix &other)
{
    this->num_vertex = other.num_vertex;
    this->is_directed = other.is_directed;
    this->matrix = nullptr;
    allocateMatrix();

    for (int i = 0; i < num_vertex; i++)
    {
        for (int j = 0; j < num_vertex; j++)
        {
            this->matrix[i][j] = other.matrix[i][j];
        }
    }
}

GraphInMatrix::GraphInMatrix(GraphInMatrix &&other) noexcept
{
    this->num_vertex = other.num_vertex;
    this->is_directed = other.is_directed;
    this->matrix = other.matrix;

    other.num_vertex = 0;
    other.is_directed = true;
    other.matrix = nullptr;
}

GraphInMatrix &GraphInMatrix::operator=(const GraphInMatrix &other)
{
    if (this == &other)
    {
        return *this;
    }

    deallocateMatrix();

    this->num_vertex = other.num_vertex;
    this->is_directed = other.is_directed;
    this->matrix = nullptr;
    allocateMatrix();

    for (int i = 0; i < num_vertex; i++)
    {
        for (int j = 0; j < num_vertex; j++)
        {
            this->matrix[i][j] = other.matrix[i][j];
        }
    }

    return *this;
}

GraphInMatrix &GraphInMatrix::operator=(GraphInMatrix &&other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    deallocateMatrix();

    this->num_vertex = other.num_vertex;
    this->is_directed = other.is_directed;
    this->matrix = other.matrix;

    other.num_vertex = 0;
    other.is_directed = true;
    other.matrix = nullptr;

    return *this;
}

GraphInMatrix::~GraphInMatrix()
{
    deallocateMatrix();
}

void GraphInMatrix::allocateMatrix()
{
    matrix = new int *[num_vertex];

    for (int i = 0; i < num_vertex; i++)
    {
        matrix[i] = new int[num_vertex];
        for (int j = 0; j < num_vertex; j++)
        {
            matrix[i][j] = 0;
        }
    }
}

void GraphInMatrix::deallocateMatrix()
{
    if (matrix == nullptr)
    {
        return;
    }

    for (int i = 0; i < num_vertex; i++)
    {
        delete[] matrix[i];
    }

    delete[] matrix;
    matrix = nullptr;
}

void GraphInMatrix::validateVertex(int v) const
{
    if (v < 0 || v >= num_vertex)
    {
        throw std::out_of_range("Vertice fora do intervalo valido.");
    }
}

int GraphInMatrix::getNumVertex() const
{
    return num_vertex;
}

bool GraphInMatrix::getIsDirected() const
{
    return is_directed;
}

int GraphInMatrix::getEdgeCount() const
{
    int count = 0;

    for (int i = 0; i < num_vertex; i++)
    {
        for (int j = 0; j < num_vertex; j++)
        {
            if (matrix[i][j] != 0)
            {
                ++count;
            }
        }
    }

    if (!is_directed)
    {
        count /= 2;
    }

    return count;
}

bool GraphInMatrix::addEdge(int u, int v)
{
    validateVertex(u);
    validateVertex(v);

    bool inserted = (matrix[u][v] == 0);
    matrix[u][v] = 1;

    if (!is_directed)
    {
        bool reverse_inserted = (matrix[v][u] == 0);
        matrix[v][u] = 1;
        inserted = inserted || reverse_inserted;
    }

    return inserted;
}

bool GraphInMatrix::removeEdge(int u, int v)
{
    validateVertex(u);
    validateVertex(v);

    bool removed = (matrix[u][v] != 0);
    matrix[u][v] = 0;

    if (!is_directed)
    {
        bool reverse_removed = (matrix[v][u] != 0);
        matrix[v][u] = 0;
        removed = removed || reverse_removed;
    }

    return removed;
}

bool GraphInMatrix::hasEdge(int u, int v) const
{
    validateVertex(u);
    validateVertex(v);
    return matrix[u][v] != 0;
}

bool GraphInMatrix::shouldIgnoreEdge(int u, int v, int ignored_u, int ignored_v) const
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

bool GraphInMatrix::dfsReachable(int current, int target, bool *visited, int ignored_u, int ignored_v) const
{
    if (current == target)
    {
        return true;
    }

    visited[current] = true;

    for (int neighbor = 0; neighbor < num_vertex; neighbor++)
    {
        if (matrix[current][neighbor] != 0 &&
            !shouldIgnoreEdge(current, neighbor, ignored_u, ignored_v) &&
            !visited[neighbor])
        {
            if (dfsReachable(neighbor, target, visited, ignored_u, ignored_v))
            {
                return true;
            }
        }
    }

    return false;
}

bool GraphInMatrix::pathExistsDFS(int source, int target, int ignored_u, int ignored_v) const
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

GraphInMatrix GraphInMatrix::transitiveReductionByDFS() const
{
    if (!is_directed)
    {
        return connectivityReductionUndirectedByDFS();
    }

    GraphInMatrix reduced(*this);

    for (int u = 0; u < num_vertex; u++)
    {
        for (int v = 0; v < num_vertex; v++)
        {
            if (this->matrix[u][v] != 0 && reduced.hasEdge(u, v))
            {
                if (reduced.pathExistsDFS(u, v, u, v))
                {
                    reduced.removeEdge(u, v);
                }
            }
        }
    }

    return reduced;
}

GraphInMatrix GraphInMatrix::transitiveReductionByWarshallForDAG() const
{
    if (!is_directed)
    {
        throw std::logic_error("A reducao transitiva por Warshall foi implementada para grafos direcionados aciclicos.");
    }

    bool **reach = new bool *[num_vertex];
    for (int i = 0; i < num_vertex; i++)
    {
        reach[i] = new bool[num_vertex];
        for (int j = 0; j < num_vertex; j++)
        {
            reach[i][j] = (matrix[i][j] != 0);
        }
    }

    for (int k = 0; k < num_vertex; k++)
    {
        for (int i = 0; i < num_vertex; i++)
        {
            for (int j = 0; j < num_vertex; j++)
            {
                reach[i][j] = reach[i][j] || (reach[i][k] && reach[k][j]);
            }
        }
    }

    GraphInMatrix reduced(*this);

    for (int u = 0; u < num_vertex; u++)
    {
        for (int v = 0; v < num_vertex; v++)
        {
            if (matrix[u][v] != 0)
            {
                bool redundant = false;

                for (int w = 0; w < num_vertex && !redundant; w++)
                {
                    if (w != v && matrix[u][w] != 0 && reach[w][v])
                    {
                        redundant = true;
                    }
                }

                if (redundant)
                {
                    reduced.removeEdge(u, v);
                }
            }
        }
    }

    for (int i = 0; i < num_vertex; i++)
    {
        delete[] reach[i];
    }
    delete[] reach;

    return reduced;
}

void GraphInMatrix::dfsBuildUndirectedForest(int current, bool *visited, GraphInMatrix &forest) const
{
    visited[current] = true;

    for (int neighbor = 0; neighbor < num_vertex; neighbor++)
    {
        if (matrix[current][neighbor] != 0 && !visited[neighbor])
        {
            forest.addEdge(current, neighbor);
            dfsBuildUndirectedForest(neighbor, visited, forest);
        }
    }
}

GraphInMatrix GraphInMatrix::connectivityReductionUndirectedByDFS() const
{
    if (is_directed)
    {
        throw std::logic_error("A reducao por floresta geradora e propria para grafos nao direcionados.");
    }

    GraphInMatrix forest(num_vertex, false);

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

bool GraphInMatrix::hasSameReachabilityAs(const GraphInMatrix &other) const
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

void GraphInMatrix::printGraphInMatrix() const
{
    std::cout << "Tipo: " << (is_directed ? "direcionado" : "nao direcionado") << '\n';
    std::cout << "Vertices: " << num_vertex << " | Arestas: " << getEdgeCount() << '\n';

    std::cout << "    ";
    for (int j = 0; j < num_vertex; j++)
    {
        std::cout << j << ' ';
    }
    std::cout << '\n';

    for (int i = 0; i < num_vertex; i++)
    {
        std::cout << i << " | ";
        for (int j = 0; j < num_vertex; j++)
        {
            std::cout << matrix[i][j] << ' ';
        }
        std::cout << '\n';
    }
}
