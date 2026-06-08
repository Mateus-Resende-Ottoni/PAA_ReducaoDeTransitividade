#include "Edge.h"

Edge::Edge(int id_neighbor)
{
    this->id_neighbor = id_neighbor;
    this->next = nullptr;
}

int Edge::getIdNeighbor() const
{
    return this->id_neighbor;
}

Edge *Edge::getNext() const
{
    return this->next;
}

void Edge::setNext(Edge *next_edge)
{
    this->next = next_edge;
}
