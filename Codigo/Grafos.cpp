#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <chrono>
#include <random>
#include <vector>
#include <utility>

#include "Bibliotecas/GraphInList/GraphInList.h"
#include "Bibliotecas/GraphInMatrix/GraphInMatrix.h"

/// Arquivo principal do trabalho.
/// Este arquivo não implementa as estruturas internas do grafo; ele monta
/// exemplos, lê grafos de arquivo, executa as abordagens disponíveis :
/// 
///    1. Redução transitiva por DFS.
///    2. Redução transitiva por Warshall/fecho transitivo para DAGs.
///    3. Redução transitiva por ordenação topológica reversa para DAGs.
///    4. Redução de conectividade por floresta geradora em grafos não direcionados.
/// 
///  Também salva resultados das baterias de testes
///  A separação é intencional: as classes GraphInList e GraphInMatrix concentram
///  a lógica algorítmica, enquanto main apenas demonstra e coordena a execução.


/// @brief Mede o tempo médio de execução de um algoritmo em microssegundos.
///
/// Executa uma iteração inicial de aquecimento (warm-up) para estabilização de cache 
/// e em seguida realiza uma bateria de testes repetidos para extrair a média temporal.
///
/// @param algoritmo Função lambda ou objeto invocável representando o algoritmo a ser testado.
/// @return Tempo médio de execução em microssegundos.
template <typename Func>
double measureRuntimeMicroseconds(Func algoritmo) {
    const int NUM_EXECUCOES = 30;

    // 1. Warm-up (Ignorar este tempo)
    algoritmo(); 

    auto inicio = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_EXECUCOES; ++i) {
        algoritmo();
    }

    // 4. Parar cronómetro e calcular média
    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> duracao_total = fim - inicio;

    return duracao_total.count() / NUM_EXECUCOES;
}

/// @brief Gera um Grafo Direcionado Acíclico (DAG) aleatório baseado em restrição topológica.
///
/// Garante a aciclicidade forçando que a origem u seja estritamente menor que o destino v.
/// Alimenta simultaneamente as estruturas de lista e matriz fornecidas.
///
/// @param V Número de vértices total do grafo.
/// @param E Número de arestas desejado para a instância.
/// @param grafoLista Referência para a estrutura de lista de adjacência.
/// @param grafoMatriz Referência para a estrutura de matriz de adjacência.
/// @throws std::invalid_argument Se o número de arestas ultrapassar o limite máximo para um DAG.
void gerarDAG(int V, int E, GraphInList& grafoLista, GraphInMatrix& grafoMatriz) {

    int maxE = V * (V - 1) / 2;

    if (E > maxE) {
        throw std::invalid_argument("Numero de arestas superior ao limite para que o grafo seja DAG");
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> dist_u(0, V - 2);
    
    int arestas_inseridas = 0;
    
    std::vector<std::vector<bool>> existe_aresta(V + 1, std::vector<bool>(V + 1, false));

    while (arestas_inseridas < E) {
        int u = dist_u(gen);
        std::uniform_int_distribution<> dist_v(u + 1, V-1); // Garante que v > u (Aciclicidade Matemática)
        int v = dist_v(gen);

        if (!existe_aresta[u][v]) {
            existe_aresta[u][v] = true;
            
            grafoLista.addEdge(u, v); 
            grafoMatriz.addEdge(u, v);
            
            arestas_inseridas++;
        }
    }
}

/// @brief Executa a bateria automatizada de testes empíricos do projeto.
///
/// Modula o comportamento dos algoritmos variando a quantidade de vértices e a 
/// densidade (esparsa e densa), exportando as médias de tempo em formato CSV.
void runExperiments() {
    std::cout << "Iniciando a bateria de testes empiricos...\n";
    
    std::ofstream csv("Resultados/resultados_paa.csv");
    if (!csv.is_open()) {
        throw std::runtime_error("Erro ao criar o arquivo CSV.");
    }
    
    // Cabeçalho
    csv << "Algoritmo,V,E,Densidade,TempoMedio_us,ArestasRemovidas\n";

    // Tamanho das instâncias
    std::vector<int> tamanhos_V = {10, 100, 500};

    for (int V : tamanhos_V) {
        int e_esparso = 2 * V;
        int e_denso = (V * (V - 1)) / 4; 
        
        std::vector<std::pair<int, std::string>> densidades = {
            {e_esparso, "Esparsa"},
            {e_denso, "Densa"}
        };

        for (const auto& densidade : densidades) {
            int E = densidade.first;
            std::string tipo_dens = densidade.second;
            
            std::cout << "Gerando DAG (V=" << V << ", E=" << E << ")..." << std::endl;

            GraphInList grafo_lista(V, true);
            GraphInMatrix grafo_matriz(V, true);
            gerarDAG(V, E, grafo_lista, grafo_matriz);

            // Medir Algoritmo 1: DFS em Lista
            double tempo_dfs_lista = measureRuntimeMicroseconds([&]() {
                return grafo_lista.transitiveReductionByDFS();
            });
            // NOTA: Para extrair as Arestas Removidas, rode o algoritmo uma vez separadamente 
            // e subtraia (Arestas Originais - Arestas Reduzidas). Aqui uso um valor dummy 'X'.
            csv << "DFS_Lista," << V << "," << E << "," << tipo_dens << "," 
                << tempo_dfs_lista << ",X\n";

            // Medir Algoritmo 2: Warshall em Matriz
            double tempo_warshall = measureRuntimeMicroseconds([&]() {
                return grafo_matriz.transitiveReductionByWarshallForDAG();
            });
            csv << "Warshall_Matriz," << V << "," << E << "," << tipo_dens << "," 
                << tempo_warshall << ",X\n";

            // Medir Algoritmo 3: Ordenação Topológica em Lista
            double tempo_top = measureRuntimeMicroseconds([&]() {
                return grafo_lista.transitiveReductionByReverseTopologicalOrder();
            });
            csv << "TopSort_Lista," << V << "," << E << "," << tipo_dens << "," 
                << tempo_top << ",X\n";
        }
    }
    
    csv.close();
    std::cout << "Experimentos concluidos com sucesso. Resultados guardados em 'resultados_paa.csv'.\n";
}

/// @brief Imprime uma linha divisória estilizada para estruturação do console.
/// @param title Texto descritivo que encabeça a seção.
void printLine(const std::string &title)
{
    std::cout << "\n============================================================\n";
    std::cout << title << '\n';
    std::cout << "============================================================\n";
}

/// @brief Exibe o veredicto da validação de alcançabilidade no fluxo padrão de saída.
/// @param ok Booleano indicando se as matrizes de fechamento transitivo coincidem.
void printReachabilityResult(bool ok)
{
    std::cout << "Preserva atingibilidade? " << (ok ? "SIM" : "NAO") << "\n";
}

/// @brief Reporta uma exceção controlada informando a omissão forçada de um algoritmo.
/// @param algorithm_name Nome do método cuja hipótese falhou.
/// @param ex Referência para a exceção capturada em tempo de execução.
void printSkippedAlgorithm(const std::string &algorithm_name, const std::exception &ex)
{
    std::cout << "\n" << algorithm_name << " nao executada: " << ex.what() << "\n";
}

/// @brief Constrói e demonstra de forma didática o cenário controlado direcionado (DAG).
///
/// Instancia um grafo predefinido contendo redundâncias explícitas e executa a 
/// validação comparativa cruzada entre todas as abordagens disponíveis.
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

/// @brief Constrói e demonstra de forma didática o cenário controlado não direcionado.
///
/// Aplica a redução baseada em conectividade para extrair uma floresta geradora DFS,
/// mitigando o problema dos ciclos lógicos de tamanho dois inerentes à bidirecionalidade.
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

/// @brief Lê e processa a topologia de um grafo a partir de um arquivo físico de entrada.
///
/// Interpreta o cabeçalho descritivo (vértices, arestas, flag direcionada) e carrega
/// dinamicamente as estruturas locais para execução imediata das reduções adequadas.
///
/// @param file_name Caminho ou nome do arquivo de texto a ser mapeado.
/// @throws std::runtime_error Se o arquivo não puder ser aberto ou violar a especificação do cabeçalho.
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
            std::string arg1 = argv[1];
            if (arg1 == "--test" || arg1 == "-exp") {
                runExperiments();
            } else {
                runFromFile(argv[1]);
            }
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
        std::cerr << "Falha critica: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}
