class GraphInMatrix
{
#ifndef GRAPHINMATRIX_H
#define GRAPHINMATRIX_H

private:
    int num_vertex;
    bool is_directed;
    int **matrix;

public:
    GraphInMatrix(int num_vertex, bool is_directed);

    ~GraphInMatrix();

    void addEdge(int u, int v);
};

#endif