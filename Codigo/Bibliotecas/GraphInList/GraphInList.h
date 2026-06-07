#ifndef GRAPHINLIST_H
#define GRAPHINLIST_H

#include "../Vertex/Vertex.h"

class GraphInList
{
private:
    int num_vertex;
    Vertex *vertex;
    bool is_directed;

public:
    GraphInList(int num_vertex, bool is_directed);

    ~GraphInList();


    void addEdge(int u, int v);
 
    void printGraphInList();
};

#endif