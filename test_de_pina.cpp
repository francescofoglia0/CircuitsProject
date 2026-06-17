#include <iostream>
#include <vector>
#include <cstdlib>
#include <set>
#include "grafi.hpp"
#include "visite.hpp"
#include "algoritmi_circuiti.hpp"
#include "randfiller.h"

using namespace std;

int get_rand_int(randfiller& rf, int min, int max) {
    vector<int> v(1);
    rf.fill(v, min, max);
    return v[0];
}

int main(void)
{
    // inizializzo i numeri casuali
    randfiller rf;
    
    // simulo 100 input diversi
    for(int k = 0; k < 100; k++)
    {
        unidirected_graph<int, double> G;
        int num_nodi = get_rand_int(rf, 5, 19);
        int num_archi = num_nodi + get_rand_int(rf, 0, 9);
        
        for(int e = 0; e < num_archi; e++)
        {
            int u = get_rand_int(rf, 1, num_nodi);
            int v = get_rand_int(rf, 1, num_nodi);
            if(u != v) G.add_edge({u, v});
        }
        
        if(G.all_nodes().empty()) continue;
        
        unidirected_graph<int, double> T;
        set<int> nodi_visitati;
        
        // genero l'albero di copertura 
        for(int nodo : G.all_nodes()) {
            if(nodi_visitati.find(nodo) == nodi_visitati.end()) {
                lifo<int> pila;
                auto albero = graph_visit(G, nodo, pila);
                for(const auto& a : albero.all_edges()) {
                    T.add_edge(a);
                    nodi_visitati.insert(a.from());
                    nodi_visitati.insert(a.to());
                }
            }
        }
        
        unidirected_graph<int, double> coalbero = G - T;
        vector<vector<int>> cicli_minimi = de_pina(G, T, coalbero);
        
        // controllo se ci sono tanti cicli quanti archi nel coalbero
        if(cicli_minimi.size() != coalbero.all_edges().size())
        {
            cout << "Errore\n";
            return EXIT_FAILURE;
        }
    }
    
    cout << "Tutto giusto\n";
    return EXIT_SUCCESS;
}