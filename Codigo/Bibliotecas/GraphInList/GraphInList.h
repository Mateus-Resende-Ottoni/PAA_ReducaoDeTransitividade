#ifndef GRAPHINLIST_H
#define GRAPHINLIST_H

#include "../Vertex/Vertex.h"
#include <vector>

/// @brief
/// GraphInList
/// Implementa grafos por lista de adjacência.
/// *
/// A representação por lista é indicada quando o grafo tende a ser esparso,
/// isto é, quando a quantidade de arestas é muito menor que V². Para o trabalho,
/// ela é útil porque permite caminhar apenas pelas arestas existentes durante
/// DFS, construção de floresta geradora e redução por ordem topológica reversa.
/// 
/// O mesmo tipo representa grafos direcionados e não direcionados. Em grafos não
/// direcionados, cada aresta {u, v} é armazenada internamente duas vezes:
/// u -> v e v -> u. Por isso getEdgeCount() divide o total por 2 nesse caso.
class GraphInList
{
private:
    int num_vertex;
    Vertex *vertex;
    bool is_directed;

    /// @brief Valida se o vértice se encontra no intervalo admissível.
    void validateVertex(int v) const;

    /// @brief Determina se a aresta atual é o alvo da remoção hipotética na DFS.
    bool shouldIgnoreEdge(int u, int v, int ignored_u, int ignored_v) const;

    /// @brief Núcleo da verificação de caminhos alternativos ignorando uma aresta (u, v).
    bool dfsReachable(int current, int target, bool *visited, int ignored_u, int ignored_v) const;

    /// @brief Constrói componentes conexos acíclicos para grafos não direcionados.
    void dfsBuildUndirectedForest(int current, bool *visited, GraphInList &forest) const;

    /// @brief Obtém os vizinhos de saída para processamento topológico.
    std::vector<int> getOutgoingNeighbors(int u) const;

    /// @brief Verifica a existência de ciclos e gera a ordenação base (Algoritmo de Kahn).
    std::vector<int> topologicalOrderByKahn() const;

public:
    GraphInList(int num_vertex, bool is_directed);
    GraphInList(const GraphInList &other);
    GraphInList(GraphInList &&other) noexcept;
    GraphInList &operator=(const GraphInList &other);
    GraphInList &operator=(GraphInList &&other) noexcept;
    ~GraphInList();

    int getNumVertex() const;
    bool getIsDirected() const;

    /// @brief Retorna o total de arestas. Ajusta a contagem (divide por 2) caso o grafo não seja direcionado.
    int getEdgeCount() const;

    /// @brief Adiciona uma aresta, garantindo a invariante de um grafo simples.
    bool addEdge(int u, int v);

    bool removeEdge(int u, int v);
    bool hasEdge(int u, int v) const;

    /// @brief Verifica a existência de caminho entre origem e destino, opcionalmente ignorando uma aresta específica.
    bool pathExistsDFS(int source, int target, int ignored_u = -1, int ignored_v = -1) const;

    /// @brief Executa redução transitiva testando a redundância de cada aresta iterativamente.
    /// @return Grafo reduzido em O(|E|(|V| + |E|)) preservando a atingibilidade original.
    GraphInList transitiveReductionByDFS() const;

    /// @brief Executa redução transitiva otimizada para DAGs esparsos.
    /// @return Grafo reduzido com custo mitigado pela reutilização de atingibilidade em conjuntos de 64 bits.
    GraphInList transitiveReductionByReverseTopologicalOrder() const;

    /// @brief Alternativa estrutural para quando a hipótese de aciclicidade falha (grafos não direcionados).
    /// @return Floresta geradora DFS que preserva puramente a conectividade simétrica.
    GraphInList connectivityReductionUndirectedByDFS() const;

    /// @brief Validação de corretude: confirma se o DAG reduzido preserva a matriz de atingibilidade original.
    bool hasSameReachabilityAs(const GraphInList &other) const;
    
    void printGraphInList() const;
};

#endif
