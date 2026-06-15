#include "Vertex.h"
#include <iostream>

Vertex::Vertex()
{
    this->id = -1;
    this->head = nullptr;
}

Vertex::~Vertex()
{
    clearEdges();
}

void Vertex::setId(int id)
{
    this->id = id;
}

int Vertex::getId() const
{
    return this->id;
}

Edge *Vertex::getHead() const
{
    return this->head;
}

bool Vertex::hasEdge(int id_neighbor) const
{
    Edge *current = head;
    while (current != nullptr)
    {
        if (current->getIdNeighbor() == id_neighbor)
        {
            return true;
        }
        current = current->getNext();
    }
    return false;
}

bool Vertex::addEdge(int id_neighbor)
{
    // Mantém o grafo simples: não insere arestas paralelas iguais.
    if (hasEdge(id_neighbor))
    {
        return false;
    }

    // Inserção no início é O(1). A ordem visual de impressão pode ficar
    // invertida em relação à ordem de leitura, mas isso não altera o grafo.
    Edge *new_edge = new Edge(id_neighbor);
    new_edge->setNext(head);
    head = new_edge;
    return true;
}

bool Vertex::removeEdge(int id_neighbor)
{
    Edge *current = head;
    Edge *previous = nullptr;

    while (current != nullptr)
    {
        if (current->getIdNeighbor() == id_neighbor)
        {
            if (previous == nullptr)
            {
                head = current->getNext();
            }
            else
            {
                previous->setNext(current->getNext());
            }

            delete current;
            return true;
        }

        previous = current;
        current = current->getNext();
    }

    return false;
}

void Vertex::clearEdges()
{
    Edge *current = head;
    while (current != nullptr)
    {
        Edge *temp = current;
        current = current->getNext();
        delete temp;
    }
    head = nullptr;
}

int Vertex::degree() const
{
    int count = 0;
    Edge *current = head;
    while (current != nullptr)
    {
        ++count;
        current = current->getNext();
    }
    return count;
}

void Vertex::printEdges() const
{
    std::cout << "Vertice " << id << " conecta com: ";

    Edge *current = head;
    if (current == nullptr)
    {
        std::cout << "(sem arestas)";
    }

    while (current != nullptr)
    {
        std::cout << current->getIdNeighbor();
        current = current->getNext();
        if (current != nullptr)
        {
            std::cout << " -> ";
        }
    }

    std::cout << '\n';
}
