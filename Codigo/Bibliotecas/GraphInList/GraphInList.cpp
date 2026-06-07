#include "GraphInList.h"

    GraphInList::GraphInList(int num_vertex, bool is_directed)
    {
        this->num_vertex = num_vertex;
        this->is_directed = is_directed;
        vertex = new Vertex[num_vertex];
        for (int i = 0; i < num_vertex; i++)
        {
            vertex[i].setId(i);
        }
    }
    
    GraphInList::~GraphInList()
    {
        delete[] vertex;
    }

    void GraphInList::addEdge(int u, int v)
    {
        vertex[u].addEdge(v);

        if (!is_directed)
        {
            vertex[v].addEdge(u);
        }
    }
    void GraphInList::printGraphInList()
    {
        for (int i = 0; i < num_vertex; i++)
        {
            vertex[i].printEdges();
        }
    }
