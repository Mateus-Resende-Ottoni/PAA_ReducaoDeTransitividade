#include "GraphInList.h"
#include <cstddef>
#include <iostream>
#include <queue>
#include <stdexcept>

namespace
{
    const int BITS_PER_WORD = 64;

    int wordCountForBits(int number_of_bits)
    {
        return (number_of_bits + BITS_PER_WORD - 1) / BITS_PER_WORD;
    }

    void setBit(std::vector<unsigned long long> &bits, int position)
    {
        bits[position / BITS_PER_WORD] |= (1ULL << (position % BITS_PER_WORD));
    }

    bool getBit(const std::vector<unsigned long long> &bits, int position)
    {
        return (bits[position / BITS_PER_WORD] & (1ULL << (position % BITS_PER_WORD))) != 0ULL;
    }

    // Otimiza o cálculo de alcançabilidade para DAGs esparsos aglutinando o estado em 
    // palavras de 64 bits.
    void unionBits(std::vector<unsigned long long> &destination,
                   const std::vector<unsigned long long> &source)
    {
        for (std::size_t i = 0; i < destination.size(); i++)
        {
            destination[i] |= source[i];
        }
    }
}

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

    // Cópia profunda: cada objeto Edge é recriado no novo grafo. Isso evita
    // dois grafos apontando para a mesma lista encadeada de arestas.
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

    // O grafo de origem perde a posse do vetor para que seu destrutor não libere
    // a memória que agora pertence ao objeto movido.
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

    if (u == v)
    {
        throw std::invalid_argument("Lacos u->u nao sao aceitos neste trabalho; use grafos simples.");
    }

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

    // Em grafo não direcionado, ignorar {u, v} significa ignorar os dois
    // registros internos u->v e v->u.
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

std::vector<int> GraphInList::getOutgoingNeighbors(int u) const
{
    validateVertex(u);

    std::vector<int> neighbors;
    Edge *edge = vertex[u].getHead();
    while (edge != nullptr)
    {
        neighbors.push_back(edge->getIdNeighbor());
        edge = edge->getNext();
    }
    return neighbors;
}

std::vector<int> GraphInList::topologicalOrderByKahn() const
{
    if (!is_directed)
    {
        throw std::logic_error("Ordenacao topologica e definida para grafos direcionados.");
    }

    std::vector<int> indegree(num_vertex, 0);
    for (int u = 0; u < num_vertex; u++)
    {
        Edge *edge = vertex[u].getHead();
        while (edge != nullptr)
        {
            indegree[edge->getIdNeighbor()]++;
            edge = edge->getNext();
        }
    }

    std::queue<int> zero_indegree;
    for (int u = 0; u < num_vertex; u++)
    {
        if (indegree[u] == 0)
        {
            zero_indegree.push(u);
        }
    }

    std::vector<int> order;
    order.reserve(num_vertex);

    while (!zero_indegree.empty())
    {
        int u = zero_indegree.front();
        zero_indegree.pop();
        order.push_back(u);

        Edge *edge = vertex[u].getHead();
        while (edge != nullptr)
        {
            int v = edge->getIdNeighbor();
            indegree[v]--;
            if (indegree[v] == 0)
            {
                zero_indegree.push(v);
            }
            edge = edge->getNext();
        }
    }


    // A ordenação topológica exige a ausência de ciclos. A validação protege a
    // integridade estrutural do algoritmo de redução subsequente.
    if (static_cast<int>(order.size()) != num_vertex)
    {
        throw std::logic_error("O grafo direcionado contem ciclo; reducao por ordem topologica reversa exige DAG.");
    }

    return order;
}

GraphInList GraphInList::transitiveReductionByDFS() const
{
    if (!is_directed)
    {
        return connectivityReductionUndirectedByDFS();
    }

    GraphInList reduced(*this);

    // Para cada aresta u->v, remove-a temporariamente do raciocínio e pergunta:
    // ainda existe caminho de u até v? Se sim, a aresta direta é redundante.
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

GraphInList GraphInList::transitiveReductionByReverseTopologicalOrder() const
{
    if (!is_directed)
    {
        throw std::logic_error("Reducao transitiva por ordem topologica reversa e propria para grafos direcionados aciclicos.");
    }

    std::vector<int> topological_order = topologicalOrderByKahn();
    int words = wordCountForBits(num_vertex);

    // reachable[u] é um conjunto de bits: reachable[u][v] = 1 significa que v
    // é alcançável a partir de u no grafo original. A ordem reversa garante que
    // todos os sucessores de u já tiveram seus conjuntos calculados.
    std::vector<std::vector<unsigned long long>> reachable(
        num_vertex, std::vector<unsigned long long>(words, 0ULL));

    for (int position = num_vertex - 1; position >= 0; position--)
    {
        int u = topological_order[position];
        std::vector<int> neighbors = getOutgoingNeighbors(u);

        for (int v : neighbors)
        {
            setBit(reachable[u], v);
            unionBits(reachable[u], reachable[v]);
        }
    }

    GraphInList reduced(num_vertex, true);

    // A aresta u->v é redundante quando existe outro sucessor direto w de u
    // que já alcança v. Nesse caso, o caminho u->w...v substitui u->v sem
    // alterar a atingibilidade de u para v nem dos demais pares.
    for (int u = 0; u < num_vertex; u++)
    {
        std::vector<int> neighbors = getOutgoingNeighbors(u);

        for (int v : neighbors)
        {
            bool redundant = false;

            for (int w : neighbors)
            {
                if (w != v && getBit(reachable[w], v))
                {
                    redundant = true;
                    break;
                }
            }

            if (!redundant)
            {
                reduced.addEdge(u, v);
            }
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

    // Se o grafo tiver mais de um componente, o laço inicia uma nova árvore em
    // cada componente. O resultado geral é uma floresta geradora.
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
