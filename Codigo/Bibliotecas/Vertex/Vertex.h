#ifndef VERTEX_H
#define VERTEX_H

#include "../Edge/Edge.h"

class Vertex
{
private:
    int id;
    Edge *head;

public:
    Vertex();

    ~Vertex();

    void setId(int id);

    int getId();

    void addEdge(int id_neighbor);

    void printEdges();

};

#endif