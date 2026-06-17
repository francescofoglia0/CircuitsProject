#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <set>
#include "grafi.hpp"
#include "visite.hpp"
#include "algoritmi_circuiti.hpp"

using namespace std;

int main(void)
{
    // inizializzo i numeri casuali
    srand(time(NULL));
    
    // ciclo su 100 input diversi
    for(int k = 0; k < 100; k++)
    {
        unidirected_graph<int, double> G;
        int num_nodi = rand() % 20 + 5;
        int num_archi = num_nodi + (rand() % 15);
        
        for(int e = 0; e < num_archi; e++)
        {
            int u = rand() % num_nodi + 1;
            int v = rand() % num_nodi + 1;
            if(u != v) G.add_edge({u, v});
        }
        
        if(G.all_nodes().empty()) continue;
        
        unidirected_graph<int, double> T;
        set<int> nodi_visitati;
        
        // genero l'albero di copertura per gestire grafi disconnessi
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
        auto cicli = dfs_cicli(G, T, coalbero);
        
        // controllo che trovi esattamente tanti cicli quante sono gli archi del coalbero
        if(cicli.size() != coalbero.all_edges().size())
        {
            cout << "Errore\n";
            return EXIT_FAILURE;
        }
    }
    
    cout << "Tutto giusto\n";
    return EXIT_SUCCESS;
}