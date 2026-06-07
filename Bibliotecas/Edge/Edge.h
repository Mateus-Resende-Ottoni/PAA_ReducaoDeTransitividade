#ifndef EDGE_H
#define EDGE_H

class Edge
{
private:
    int id_neighbor;
    Edge *next;

public:
    ~Edge() {} 

    Edge(int id_neighbor);

    int getIdNeighbor();

    Edge *getNext();

    void setNext(Edge *next_edge);
};

#endif
