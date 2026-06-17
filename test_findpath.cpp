#include <iostream>
#include <vector>
#include <set>
#include <cstdlib>
#include <ctime>
#include "grafi.hpp"
#include "algoritmi_circuiti.hpp"

using namespace std;

int main(void)
{
    // inizializzo il seed per i numeri casuali
    srand(time(NULL));
    
    // faccio 100 test con alberi casuali di dimensioni diverse
    for(int k = 0; k < 100; k++)
    {
        unidirected_graph<int, double> T;
        int num_nodi = rand() % 50 + 5;
        
        // costruisco un albero casuale collegando ogni nodo a un padre precedente
        for(int i = 2; i <= num_nodi; i++)
        {
            int padre = rand() % (i - 1) + 1;
            T.add_edge({padre, i});
        }
        
        set<int> visited;
        vector<int> path;
        
        // scelgo due nodi a caso per testare se trova il cammino
        int partenza = rand() % num_nodi + 1;
        int arrivo = rand() % num_nodi + 1;
        
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