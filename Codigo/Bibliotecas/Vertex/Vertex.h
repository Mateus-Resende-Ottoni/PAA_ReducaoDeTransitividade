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

    Vertex(const Vertex &) = delete;
    Vertex &operator=(const Vertex &) = delete;

    void setId(int id);
    int getId() const;

    Edge *getHead() const;

    bool hasEdge(int id_neighbor) const;
    bool addEdge(int id_neighbor);
    bool removeEdge(int id_neighbor);
    void clearEdges();

    int degree() const;
    void printEdges() const;
};

#endif
