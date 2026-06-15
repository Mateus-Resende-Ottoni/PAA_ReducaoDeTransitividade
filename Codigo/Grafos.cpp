#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "Bibliotecas/GraphInList/GraphInList.h"
#include "Bibliotecas/GraphInMatrix/GraphInMatrix.h"

/*
 * Arquivo principal do trabalho.
 * -----------------------------
 * Este arquivo não implementa as estruturas internas do grafo; ele monta
 * exemplos, lê grafos de arquivo e executa as abordagens disponíveis:
 *
 *   1. Redução transitiva por DFS.
 *   2. Redução transitiva por Warshall/fecho transitivo para DAGs.
 *   3. Redução transitiva por ordenação topológica reversa para DAGs.
 *   4. Redução de conectividade por floresta geradora em grafos não direcionados.
 *
 * A separação é intencional: as classes GraphInList e GraphInMatrix concentram
 * a lógica algorítmica, enquanto main apenas demonstra e coordena a execução.
 */

void printLine(const std::string &title)
{
    std::cout << "\n============================================================\n";
    std::cout << title << '\n';
    std::cout << "============================================================\n";
}

void printReachabilityResult(bool ok)
{
    std::cout << "Preserva atingibilidade? " << (ok ? "SIM" : "NAO") << "\n";
}

void printSkippedAlgorithm(const std::string &algorithm_name, const std::exception &ex)
{
    std::cout << "\n" << algorithm_name << " nao executada: " << ex.what() << "\n";
}

void runDirectedExample()
{
    printLine("EXEMPLO 1 - Grafo direcionado com arestas transitivas");

    GraphInList list_graph(6, true);
    GraphInMatrix matrix_graph(6, true);

    int edges[][2] = {
        {0, 1}, {0, 2}, {0, 3}, {1, 2}, {1, 3}, {2, 3}, {2, 4}, {3, 4}, {4, 5}, {0, 5}, {1, 5}};

    int edge_count = sizeof(edges) / sizeof(edges[0]);
    for (int i = 0; i < edge_count; i++)
    {
        list_graph.addEdge(edges[i][0], edges[i][1]);
        matrix_graph.addEdge(edges[i][0], edges[i][1]);
    }

    std::cout << "\nGrafo original em lista de adjacencia:\n";
    list_graph.printGraphInList();

    GraphInList reduced_list_dfs = list_graph.transitiveReductionByDFS();
    std::cout << "\nReducao transitiva por DFS com lista de adjacencia:\n";
    reduced_list_dfs.printGraphInList();
    printReachabilityResult(list_graph.hasSameReachabilityAs(reduced_list_dfs));

    try
    {
        GraphInList reduced_list_topological = list_graph.transitiveReductionByReverseTopologicalOrder();
        std::cout << "\nReducao transitiva por ordenacao topologica reversa com lista de adjacencia:\n";
        reduced_list_topological.printGraphInList();
        printReachabilityResult(list_graph.hasSameReachabilityAs(reduced_list_topological));
    }
    catch (const std::exception &ex)
    {
        printSkippedAlgorithm("Reducao por ordem topologica reversa/lista", ex);
    }

    std::cout << "\nGrafo original em matriz de adjacencia:\n";
    matrix_graph.printGraphInMatrix();

    GraphInMatrix reduced_matrix_dfs = matrix_graph.transitiveReductionByDFS();
    std::cout << "\nReducao transitiva por DFS com matriz de adjacencia:\n";
    reduced_matrix_dfs.printGraphInMatrix();
    printReachabilityResult(matrix_graph.hasSameReachabilityAs(reduced_matrix_dfs));

    try
    {
        GraphInMatrix reduced_matrix_warshall = matrix_graph.transitiveReductionByWarshallForDAG();
        std::cout << "\nReducao transitiva por Warshall/fecho transitivo para DAG:\n";
        reduced_matrix_warshall.printGraphInMatrix();
        printReachabilityResult(matrix_graph.hasSameReachabilityAs(reduced_matrix_warshall));
    }
    catch (const std::exception &ex)
    {
        printSkippedAlgorithm("Reducao por Warshall/matriz", ex);
    }

    try
    {
        GraphInMatrix reduced_matrix_topological = matrix_graph.transitiveReductionByReverseTopologicalOrder();
        std::cout << "\nReducao transitiva por ordenacao topologica reversa com matriz de adjacencia:\n";
        reduced_matrix_topological.printGraphInMatrix();
        printReachabilityResult(matrix_graph.hasSameReachabilityAs(reduced_matrix_topological));
    }
    catch (const std::exception &ex)
    {
        printSkippedAlgorithm("Reducao por ordem topologica reversa/matriz", ex);
    }
}

void runUndirectedExample()
{
    printLine("EXEMPLO 2 - Grafo nao direcionado reduzido para floresta geradora");

    GraphInList list_graph(7, false);
    GraphInMatrix matrix_graph(7, false);

    int edges[][2] = {
        {0, 1}, {0, 2}, {1, 2}, {1, 3}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {4, 6}};

    int edge_count = sizeof(edges) / sizeof(edges[0]);
    for (int i = 0; i < edge_count; i++)
    {
        list_graph.addEdge(edges[i][0], edges[i][1]);
        matrix_graph.addEdge(edges[i][0], edges[i][1]);
    }

    std::cout << "\nGrafo original em lista de adjacencia:\n";
    list_graph.printGraphInList();

    GraphInList forest_list = list_graph.connectivityReductionUndirectedByDFS();
    std::cout << "\nReducao de conectividade por DFS com lista de adjacencia:\n";
    forest_list.printGraphInList();
    printReachabilityResult(list_graph.hasSameReachabilityAs(forest_list));

    std::cout << "\nGrafo original em matriz de adjacencia:\n";
    matrix_graph.printGraphInMatrix();

    GraphInMatrix forest_matrix = matrix_graph.connectivityReductionUndirectedByDFS();
    std::cout << "\nReducao de conectividade por DFS com matriz de adjacencia:\n";
    forest_matrix.printGraphInMatrix();
    printReachabilityResult(matrix_graph.hasSameReachabilityAs(forest_matrix));
}

void runFromFile(const std::string &file_name)
{
    std::ifstream input(file_name.c_str());
    if (!input)
    {
        throw std::runtime_error("Nao foi possivel abrir o arquivo de entrada: " + file_name);
    }

    int n = 0;
    int m = 0;
    int directed_flag = 0;

    input >> n >> m >> directed_flag;
    if (!input)
    {
        throw std::runtime_error("Cabecalho invalido. Use: n m direcionado(0/1)");
    }

    if (n < 0 || m < 0)
    {
        throw std::runtime_error("Cabecalho invalido: n e m devem ser nao negativos.");
    }

    if (directed_flag != 0 && directed_flag != 1)
    {
        throw std::runtime_error("Cabecalho invalido: direcionado deve ser 0 ou 1.");
    }

    bool directed = (directed_flag == 1);
    GraphInList list_graph(n, directed);
    GraphInMatrix matrix_graph(n, directed);

    for (int i = 0; i < m; i++)
    {
        int u = 0;
        int v = 0;
        input >> u >> v;

        if (!input)
        {
            throw std::runtime_error("Aresta invalida no arquivo de entrada.");
        }

        list_graph.addEdge(u, v);
        matrix_graph.addEdge(u, v);
    }

    printLine("Grafo carregado de arquivo");
    std::cout << "Arquivo: " << file_name << "\n";

    std::cout << "\nRepresentacao em lista:\n";
    list_graph.printGraphInList();

    std::cout << "\nRepresentacao em matriz:\n";
    matrix_graph.printGraphInMatrix();

    if (directed)
    {
        GraphInList reduced_list_dfs = list_graph.transitiveReductionByDFS();
        GraphInMatrix reduced_matrix_dfs = matrix_graph.transitiveReductionByDFS();

        std::cout << "\nReducao transitiva por DFS/lista:\n";
        reduced_list_dfs.printGraphInList();
        printReachabilityResult(list_graph.hasSameReachabilityAs(reduced_list_dfs));

        std::cout << "\nReducao transitiva por DFS/matriz:\n";
        reduced_matrix_dfs.printGraphInMatrix();
        printReachabilityResult(matrix_graph.hasSameReachabilityAs(reduced_matrix_dfs));

        try
        {
            GraphInMatrix reduced_warshall = matrix_graph.transitiveReductionByWarshallForDAG();
            std::cout << "\nReducao por Warshall/fecho transitivo para DAG:\n";
            reduced_warshall.printGraphInMatrix();
            printReachabilityResult(matrix_graph.hasSameReachabilityAs(reduced_warshall));
        }
        catch (const std::exception &ex)
        {
            printSkippedAlgorithm("Reducao por Warshall/matriz", ex);
        }

        try
        {
            GraphInList reduced_list_topological = list_graph.transitiveReductionByReverseTopologicalOrder();
            std::cout << "\nReducao por ordenacao topologica reversa/lista:\n";
            reduced_list_topological.printGraphInList();
            printReachabilityResult(list_graph.hasSameReachabilityAs(reduced_list_topological));
        }
        catch (const std::exception &ex)
        {
            printSkippedAlgorithm("Reducao por ordem topologica reversa/lista", ex);
        }

        try
        {
            GraphInMatrix reduced_matrix_topological = matrix_graph.transitiveReductionByReverseTopologicalOrder();
            std::cout << "\nReducao por ordenacao topologica reversa/matriz:\n";
            reduced_matrix_topological.printGraphInMatrix();
            printReachabilityResult(matrix_graph.hasSameReachabilityAs(reduced_matrix_topological));
        }
        catch (const std::exception &ex)
        {
            printSkippedAlgorithm("Reducao por ordem topologica reversa/matriz", ex);
        }
    }
    else
    {
        GraphInList forest_list = list_graph.connectivityReductionUndirectedByDFS();
        GraphInMatrix forest_matrix = matrix_graph.connectivityReductionUndirectedByDFS();

        std::cout << "\nReducao de conectividade por DFS/lista:\n";
        forest_list.printGraphInList();
        printReachabilityResult(list_graph.hasSameReachabilityAs(forest_list));

        std::cout << "\nReducao de conectividade por DFS/matriz:\n";
        forest_matrix.printGraphInMatrix();
        printReachabilityResult(matrix_graph.hasSameReachabilityAs(forest_matrix));
    }
}

int main(int argc, char **argv)
{
    try
    {
        if (argc >= 2)
        {
            runFromFile(argv[1]);
        }
        else
        {
            runDirectedExample();
            runUndirectedExample();

            printLine("Como testar com arquivo proprio");
            std::cout << "Formato do arquivo:\n";
            std::cout << "n m direcionado(0/1)\n";
            std::cout << "u1 v1\n";
            std::cout << "u2 v2\n";
            std::cout << "...\n";
            std::cout << "\nExemplo de execucao: ./paa_transitividade exemplos/direcionado.txt\n";
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Erro: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}
