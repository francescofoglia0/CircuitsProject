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
        //prendiamo un numero di nodi e archi random
        int num_nodi = get_rand_int(rf, 5, 19);
        int num_archi = num_nodi + get_rand_int(rf, 0, 9);
        
        for(int e = 0; e < num_archi; e++)
        {
            int u = get_rand_int(rf, 1, num_nodi);
            int v = get_rand_int(rf, 1, num_nodi);
            //aggiungiamo l'arco solo se i nodi che lo compongono sono diversi
            //altrimenti il programma da errore in automatico
            if(u != v) G.add_edge({u, v});
        }
        //se sono in uno sfortunato ma possibile caso in cui l'albero è vuoto (i nodi erano tutti 
        // uguali a due a due) passo alla prossima iterazione
        if(G.all_nodes().empty()) continue;
        
        unidirected_graph<int, double> T;
        set<int> nodi_visitati;
        
        // genero l'albero di copertura con la dfs
        for(int nodo : G.all_nodes()) {
            if(nodi_visitati.find(nodo) == nodi_visitati.end()) {
                lifo<int> pila;
                //prendo l'albero generato dalla dfs
                auto albero = graph_visit(G, nodo, pila);
                //costruisco l'albero T e segno i nodi visitati
                for(const auto& a : albero.all_edges()) {
                    T.add_edge(a);
                    nodi_visitati.insert(a.from());
                    nodi_visitati.insert(a.to());
                }
            }
        }
        //prendo anche il colabero che dovro passare a de pina
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