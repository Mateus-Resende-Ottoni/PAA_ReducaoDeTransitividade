#ifndef EDGE_H
#define EDGE_H

/*
 * Classe Edge
 * -----------
 * Representa um nó da lista simplesmente encadeada de adjacência.
 *
 * Neste projeto, uma aresta armazenada dentro do vértice u guarda apenas o
 * identificador do vizinho v. Assim, a existência do objeto Edge dentro da
 * lista de u significa que há uma aresta u -> v no caso direcionado, ou uma
 * das duas metades da aresta {u, v} no caso não direcionado.
 *
 * A classe é pequena de propósito: ela não conhece o vértice de origem,
 * não conhece o grafo inteiro e não decide se o grafo é direcionado. Essas
 * responsabilidades pertencem a Vertex e GraphInList.
 */
class Edge
{
private:
    int id_neighbor; // Identificador do vértice de destino/vizinho.
    Edge *next;      // Próxima aresta na lista encadeada do mesmo vértice.

public:
    explicit Edge(int id_neighbor);

    int getIdNeighbor() const;
    Edge *getNext() const;
    void setNext(Edge *next_edge);
};

#endif
