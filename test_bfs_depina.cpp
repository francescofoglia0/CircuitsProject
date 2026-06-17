#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "grafi.hpp"
#include "algoritmi_circuiti.hpp"

using namespace std;

int main(void)
{
    // inizializzo i numeri casuali
    srand(time(NULL));
    
    // faccio 100 test
    for(int k = 0; k < 100; k++)
    {
        unidirected_graph<int, double> G;
        int num_nodi = rand() % 20 + 5;
        
        // creo una strada principale (da 1 fino all'ultimo nodo)
        for(int i = 1; i < num_nodi; i++)
        {
            G.add_edge({i, i + 1});
        }
        
        // aggiungo anche un po' di archi per testare la robustezza della BFS
        int bivi = rand() % 10;
        for(int i = 0; i < bivi; i++)
        {
            int u = rand() % num_nodi + 1;
            int v = rand() % num_nodi + 1;
            if(u != v) G.add_edge({u, v});
        }
        
        // lancio la BFS
        vector<int> path = bfs_depina(G, 1, num_nodi);
        
        // avendo costruito io la strada, deve trovarla per forza
        if(path.empty())
        {
            cout << "Errore\n";
            return EXIT_FAILURE;
        }
        
        // verifico che il cammino sia valido controllando che gli archi esistano davvero
        for(size_t i = 0; i < path.size() - 1; i++)
        {
            unidirected_edge<int> arco = {path[i], path[i+1]};
            if(G.edge_number(arco) == -1)
            {
                cout << "Errore\n";
                return EXIT_FAILURE;
            }
        }
    }
    
    cout << "Tutto giusto\n";
    return EXIT_SUCCESS;
}