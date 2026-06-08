#ifndef EDGE_H
#define EDGE_H

class Edge
{
private:
    int id_neighbor;
    Edge *next;

public:
    explicit Edge(int id_neighbor);

    int getIdNeighbor() const;
    Edge *getNext() const;
    void setNext(Edge *next_edge);
};

#endif
