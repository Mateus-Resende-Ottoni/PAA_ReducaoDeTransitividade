#include "Vertex.h"
#include<iostream>

Vertex::Vertex()
{
    id = -1;
    head = nullptr;
}
Vertex::~Vertex()
{
    Edge *current = head;
    while (current != nullptr)
    {
        Edge *temp = current;
        current = current->getNext();
        delete temp;
    }
}

void Vertex::setId(int id)
{
    this->id = id;
}
int Vertex::getId()
{
    return this->id;
}

void Vertex::addEdge(int id_neighbor)
{
    Edge *new_edge = new Edge(id_neighbor);
    new_edge->setNext(head);
    head = new_edge;
}

void Vertex::printEdges()
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
