#include "Edge.h"

Edge::Edge(int id_neighbor)
{
    this->id_neighbor = id_neighbor;
    next = nullptr;
}

int Edge::getIdNeighbor()
{
    return this->id_neighbor;
}
Edge *Edge::getNext()
{
    return this->next;
}
void Edge::setNext(Edge *next_edge)
{
    this->next;
}
