#ifndef VERTEX_H
#define VERTEX_H

#include "../Edge/Edge.h"

/// @brief
/// Classe Vertex
/// Representa um vértice em uma lista de adjacência manual, isto é, sem usar
/// std::list ou std::vector para as arestas.
/// 
/// Cada Vertex possui:
/// 
///    - um id, usado apenas para identificação e impressão;
/// 
///    - um ponteiro head, que aponta para a primeira aresta da lista encadeada.
/// 
/// Como a classe gerencia memória dinamicamente (objetos Edge alocados com
///  new), ela possui destrutor próprio. A cópia foi desabilitada para evitar
///  cópias rasas perigosas de ponteiros; a cópia profunda do grafo é feita em
///  GraphInList, onde existe contexto suficiente para reconstruir as adjacências.
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

    /// @brief Define o identificador do vértice.
    /// @param id Identificador numérico.
    void setId(int id);

    /// @brief Retorna o identificador do vértice.
    /// @return Identificador numérico.
    int getId() const;

    /// @brief Retorna o ponteiro para a primeira aresta.
    /// @return Ponteiro para o início da lista encadeada de arestas.
    Edge *getHead() const;

    /// @brief Verifica a existência de uma aresta direcionada.
    /// @param id_neighbor ID do vértice destino.
    /// @return true se a aresta existir, false caso contrário.
    bool hasEdge(int id_neighbor) const;

    /// @brief Adiciona uma aresta direcionada ao vértice.
    /// @param id_neighbor ID do vértice destino.
    /// @return true se inserido, false se a aresta já existir (viola grafo simples).
    bool addEdge(int id_neighbor);

    /// @brief Remove uma aresta direcionada específica.
    /// @param id_neighbor ID do vértice destino.
    /// @return true se removida, false se não encontrada.
    bool removeEdge(int id_neighbor);

    /// @brief Libera a memória de todas as arestas associadas.
    void clearEdges();

    /// @brief Calcula o grau de saída do vértice.
    /// @return Número de arestas originadas neste vértice.
    int degree() const;

    /// @brief Imprime as adjacências do vértice no fluxo de saída padrão.
    void printEdges() const;
};

#endif
