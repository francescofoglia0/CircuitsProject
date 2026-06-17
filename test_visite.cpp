#include <iostream>
#include <cstdlib>
#include <ctime>
#include "grafi.hpp"
#include "lifofifo.hpp"
#include "visite.hpp"

using namespace std;

int main(void)
{
    // inizializzo i numeri casuali
    srand(time(NULL));

    // 100 iterazioni di testing 
    for(int k = 0; k < 100; k++)
    {
        unidirected_graph<int, double> G;
        int num_nodi = rand() % 20 + 5;

        // creo prima un grafo strettamente connesso 
        for(int i = 1; i < num_nodi; i++) {
            G.add_edge({i, i + 1});
            G.add_peso({i, i + 1}, (rand() % 10) + 1.0); // peso positivo per dijkstra
        }

        // aggiungo archi extra casuali per creare maglie e altri archi (max 10 archi extra)
        int extra = rand() % 10;
        for(int i = 0; i < extra; i++) {
            int u = rand() % num_nodi + 1;
            int v = rand() % num_nodi + 1;
            if(u != v) {
                G.add_edge({u, v});
                G.add_peso({u, v}, (rand() % 10) + 1.0);
            }
        }

        // test graph_visit con lifo (dfs iterativa)
        lifo<int> pila;
        auto albero_dfs = graph_visit(G, 1, pila);
        // un albero di copertura per grafi connessi ha nodi - 1 archi
        if(albero_dfs.all_edges().size() != (size_t)(num_nodi - 1)) {
            cout << "Errore\n";
            return EXIT_FAILURE;
        }

        // test graph_visit con fifo (bfs ampiezza)
        fifo<int> coda;
        auto albero_bfs = graph_visit(G, 1, coda);
        if(albero_bfs.all_edges().size() != (size_t)(num_nodi - 1)) {
            cout << "Errore\n";
            return EXIT_FAILURE;
        }

        // test dfs ricorsiva pura
        auto albero_ric = recursive_dfs(G, 1);
        if(albero_ric.all_edges().size() != (size_t)(num_nodi - 1)) {
            cout << "Errore\n";
            return EXIT_FAILURE;
        }

        // test algoritmo di dijkstra
        auto albero_dijkstra = dijkstra(G, 1);
        // l'albero dei cammini minimi da una sorgente copre tutto il grafo (se connesso)
        if(albero_dijkstra.all_edges().size() != (size_t)(num_nodi - 1)) {
            cout << "Errore\n";
            return EXIT_FAILURE;
        }
    }

    cout << "Tutto giusto\n";
    return EXIT_SUCCESS;
}