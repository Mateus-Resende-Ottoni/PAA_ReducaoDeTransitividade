#include "GraphInMatrix.h"

GraphInMatrix::GraphInMatrix(int num_vertex, bool is_directed)
{
    this->num_vertex = num_vertex;
    this->is_directed = is_directed;

    matrix = new int *[num_vertex];

    for (int i = 0; i < num_vertex; i++)
    {
        matrix[i] = new int[num_vertex];

        for (int j = 0; j < num_vertex; j++)
        {
            matrix[i][j] = 0;
        }
    }
}

GraphInMatrix::~GraphInMatrix()
{
    for (int i = 0; i < num_vertex; i++)
    {
        delete[] matrix[i];
    }
    delete[] matrix;
}

void GraphInMatrix::addEdge(int u, int v)
{
    matrix[u][v] = 1;
    if (!is_directed)
    {
        matrix[v][u] = 1;
    }
}
