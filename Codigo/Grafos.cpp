#include <iostream>

// Galera, criei esse código inicial, totalmente feito a mão sem estruturas tipo vector e o mais legível possível
// Aqui tem grafos implementados tanto em lista (pra grafos esparsos) quando em matriz (pra grafos densos)
// Tem alguns ajustes a serem feitos, mas a ideia é transformar essas classes em biblioteca para usarmos em outros códigos se necessários e de modo geral, está pronto.

class Edge
{

private:
    int id_neighbor;
    Edge *next;

public:
    Edge(int id_neighbor)
    {
        this->id_neighbor = id_neighbor;
        next = nullptr;
    }
    ~Edge()
    {
    }
    int getIdNeighbor()
    {
        return this->id_neighbor;
    }
    Edge *getNext()
    {
        return this->next;
    }
    void setNext(Edge *next_edge)
    {
        this->next;
    }
};

class Vertex
{
private:
    int id;
    Edge *head;

public:
    Vertex()
    {
        id = -1;
        head = nullptr;
    }
    ~Vertex()
    {
        Edge *current = head;
        while (current != nullptr)
        {
            Edge *temp = current;
            current = current->getNext();
            delete temp;
        }
    }
    void setId(int id)
    {
        this->id = id;
    }
    int getId()
    {
        return this->id;
    }
    void addEdge(int id_neighbor)
    {
        Edge *new_edge = new Edge(id_neighbor);
        new_edge->setNext(head);
        head = new_edge;
    }
    void printEdges()
    {
        std::cout << "Vertice " << id << " conecta com: ";
        Edge *current = head;
        while (current != nullptr)
        {
            std::cout << current->getIdNeighbor() << " -> ";
            current = current->getNext();
        }
        std::cout << "\n";
    }
};

class GraphInList
{
private:
    int num_vertex;
    Vertex *vertex;
    bool is_directed;

public:
    GraphInList(int num_vertex, bool is_directed)
    {
        this->num_vertex = num_vertex;
        this->is_directed = is_directed;
        vertex = new Vertex[num_vertex];
        for (int i = 0; i < num_vertex; i++)
        {
            vertex[i].setId(i);
        }
    }
    ~GraphInList()
    {
        delete[] vertex;
    }

    void addEdge(int u, int v)
    {
        vertex[u].addEdge(v);

        if (!is_directed)
        {
            vertex[v].addEdge(u);
        }
    }
    void printGraphInList()
    {
        for (int i = 0; i < num_vertex; i++)
        {
            vertex[i].printEdges();
        }
    }
};

class GraphInMatrix
{
private:
    int num_vertex;
    bool is_directed;
    int **matrix;

public:
    GraphInMatrix(int num_vertex, bool is_directed)
    {
        this->num_vertex = num_vertex;
        this->is_directed = is_directed;

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

    ~GraphInMatrix()
    {
        for (int i = 0; i < num_vertex; i++)
        {
            delete[] matrix[i];
        }
        delete[] matrix;
    }

    void addEdge(int u, int v)
    {
        matrix[u][v] = 1;
        if (!is_directed)
        {
            matrix[v][u] = 1;
        }
    }
};

int main(void)
{
    GraphInList g(5, true);
    g.addEdge(0, 1);
    g.addEdge(0, 4);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.printGraphInList();

    return 0;
}