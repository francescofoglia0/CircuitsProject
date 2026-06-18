#include <iostream>
#include <vector>
#include <set>
#include <cstdlib>
#include "grafi.hpp"
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
    randfiller rf;
    
    // faccio 100 test con alberi casuali di dimensioni diverse
    for(int k = 0; k < 100; k++)
    {
        unidirected_graph<int, double> T;
        int num_nodi = get_rand_int(rf, 5, 54);
        
        // costruisco un albero casuale collegando ogni nodo a un padre precedente
        for(int i = 2; i <= num_nodi; i++)
        {
            int padre = get_rand_int(rf, 1, i - 1);
            T.add_edge({padre, i});
        }
        
        set<int> visited;
        vector<int> path;
        
        // controllo se il cammino è valido e se parte/finisce nei nodi giusti.
        // scelgo due nodi a caso per testare se trova il cammino
        int partenza = get_rand_int(rf, 1, num_nodi);
        int arrivo = get_rand_int(rf, 1, num_nodi);
        
        bool trovato = findpath(T, partenza, arrivo, visited, path);
        
        // in un albero connesso c'è sempre un cammino tra due nodi qualsiasi
        if(!trovato || path.empty() || path.front() != partenza || path.back() != arrivo)
        {
            cout << "Errore\n";
            return EXIT_FAILURE;
        }
    }
    
    cout << "Tutto giusto\n";
    return EXIT_SUCCESS;
}