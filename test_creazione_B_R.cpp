#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <set>
#include <Eigen/Dense>
#include "parser.hpp"
#include "visite.hpp"
#include "algoritmi_circuiti.hpp"
#include "sistemi_lineari.hpp"

using namespace std;

int main(void)
{
    // inizializzo il generatore casuale
    srand(time(NULL));
    string nome_file = "filetestbr.txt";

    // ciclo di stress test su 100 circuiti
    for(int k = 0; k < 100; k++)
    {
        ofstream out(nome_file);
        set<pair<int, int>> archi;
        int num_nodi = rand() % 10 + 5;
        int num_archi = num_nodi + (rand() % 10);
        int id_comp = 1;
        
        for(int e = 0; e < num_archi; e++)
        {
            int u = rand() % num_nodi + 1;
            int v = rand() % num_nodi + 1;
            if(u == v || archi.count({min(u,v), max(u,v)})) continue;
            archi.insert({min(u,v), max(u,v)});
            
            // uso solo resistenze per evitare il crash (exit(1)) sui cortocircuiti
            char tipo = 'R'; 
            double val = (rand() % 100) + 1.0;
            out << tipo << id_comp++ << " " << val << " " << u << " " << v << "\n";
        }
        out.close();

        auto [circuito, mappa] = leggi_circuito(nome_file);
        if(circuito.get_resistenze() == 0) continue;

        unidirected_graph<int,double> T;
        set<int> nodi_visitati;
        
        // costuisco l'albero di copertura
        for(int nodo : circuito.all_nodes()) {
            if(nodi_visitati.find(nodo) == nodi_visitati.end()) {
                lifo<int> pila;
                auto albero = graph_visit(circuito, nodo, pila);
                for(const auto& a : albero.all_edges()) {
                    T.add_edge(a);
                    nodi_visitati.insert(a.from());
                    nodi_visitati.insert(a.to());
                }
            }
        }
        auto coalbero = circuito - T;
        auto cicli = dfs_cicli(circuito, T, coalbero);
        if(cicli.empty()) continue;

        auto [B, R, v_gen] = creazione_B_R(circuito, cicli);

        int m = circuito.get_resistenze();
        int n = cicli.size();
        
        if(B.rows() != m || B.cols() != n || R.rows() != m || R.cols() != m || v_gen.size() != n)
        {
            cout << "Errore\n";
            return EXIT_FAILURE;
        }

        // verifico che la matrice R sia diagonale
        for(int i = 0; i < m; i++)
        {
            for(int j = 0; j < m; j++)
            {
                // i rami aperti avranno 0 in R(i,i), quindi non controllo che sia >0
                if(i != j && R(i, j) != 0) return EXIT_FAILURE;
            }
        }
        
        for(int i = 0; i < m; i++)
        {
            for(int j = 0; j < n; j++)
            {
                double val = B(i,j);
                if(val != 1.0 && val != -1.0 && val != 0.0) return EXIT_FAILURE;
            }
        }
    }
    remove(nome_file.c_str());
    cout << "Tutto giusto\n";
    return EXIT_SUCCESS;
}