#include <iostream>
#include <cstdlib>
#include <vector>
#include "grafi.hpp"
#include "lifofifo.hpp"
#include "visite.hpp"
#include "randfiller.h"

using namespace std;

int get_rand_int(randfiller& rf, int min, int max) {
    vector<int> v(1);
    rf.fill(v, min, max);
    return v[0];
}

double get_rand_double(randfiller& rf, double min, double max) {
    vector<double> v(1);
    rf.fill(v, min, max);
    return v[0];
}

int main(void)
{
    randfiller rf;

    // 100 iterazioni di testing 
    for(int k = 0; k < 100; k++)
    {
        unidirected_graph<int, double> G;
        int num_nodi = get_rand_int(rf, 5, 24);

        // creo prima un grafo strettamente connesso 
        for(int i = 1; i < num_nodi; i++) {
            G.add_edge({i, i + 1});
            G.add_peso({i, i + 1}, get_rand_double(rf, 1.0, 10.0)); // peso positivo per dijkstra
        }

        // aggiungo archi extra casuali per creare maglie e altri archi 
        int extra = get_rand_int(rf, 0, 9);
        for(int i = 0; i < extra; i++) {
            int u = get_rand_int(rf, 1, num_nodi);
            int v = get_rand_int(rf, 1, num_nodi);
            //aggiungiamo archi solo se i nodi che lo formano sono diversi
            //altrmenti otteniamo errore dal nostro programma
            if(u != v) {
                G.add_edge({u, v});
                G.add_peso({u, v}, get_rand_double(rf, 1.0, 10.0));
            }
        }

        // test graph_visit con lifo (dfs iterativa)
        lifo<int> pila;
        auto albero_dfs = graph_visit(G, 1, pila);
        // un albero di copertura per grafi connessi ha nodi - 1 archi
        size_t numero_nodi = num_nodi -1;
        if(albero_dfs.all_edges().size() != (numero_nodi)) {
            cout << "Errore\n";
            return EXIT_FAILURE;
        }
    }

    cout << "Tutto giusto\n";
    return EXIT_SUCCESS;
}