#ifndef VERTEX_H
#define VERTEX_H

#include "../Edge/Edge.h"

/*
 * Classe Vertex
 * -------------
 * Representa um vértice em uma lista de adjacência manual, isto é, sem usar
 * std::list ou std::vector para as arestas.
 *
 * Cada Vertex possui:
 *   - um id, usado apenas para identificação e impressão;
 *   - um ponteiro head, que aponta para a primeira aresta da lista encadeada.
 *
 * Como a classe gerencia memória dinamicamente (objetos Edge alocados com
 * new), ela possui destrutor próprio. A cópia foi desabilitada para evitar
 * cópias rasas perigosas de ponteiros; a cópia profunda do grafo é feita em
 * GraphInList, onde existe contexto suficiente para reconstruir as adjacências.
 */
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
