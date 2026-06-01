#include <iostream>
#include <string>
#include "grafi.hpp" 
#include "parser.hpp" 
#include "grafodot.hpp"
#include "visite.hpp"
#include "lifofifo.hpp"
#include "algoritmi_circuiti.hpp"

using namespace std;

int main(int argc, const char *argv[])
{
    if (argc < 2)  // verifico che sia stato passato il parametro da terminale
    {
        cerr << "Non hai inserito il nome del file da leggere\n"; 
        return 1;
    }
    
    string nomefile = argv[1];

    // Specifichiamo i template <int, double> per coerenza con la funzione leggi_circuito.
    unidirected_graph<int, double> circuito = leggi_circuito(nomefile);
    esporta_graphviz(circuito, "prova.dot");

    lifo<int> pila;
    unidirected_graph<int,double> albdfs = graph_visit(circuito,1,pila);
    esporta_graphviz(albdfs,"dfs.dot");

    unidirected_graph<int,double> coalbero = circuito-albdfs;
    esporta_graphviz(coalbero,"coalbero.dot");

    if(circuito.all_nodes().empty())
    {
        cerr<<"Il grafo passato è vuoto"<<endl;
        return 1;
    }
    int sorgente = *(circuito.all_nodes().begin());
    lifo<int> altrapila;
    unidirected_graph<int,double> T = graph_visit(circuito,sorgente,altrapila);
    unidirected_graph<int,double> coalbero2 = circuito-T;

    cout << "\n=== MAGLIE FONDAMENTALI (METODO DFS) ===\n";
    vector<vector<int>> cicli = dfs_cicli(circuito,T,coalbero2);
    
    for (size_t i = 0; i < cicli.size(); ++i) {
        cout << "Maglia " << i + 1 << ": ";
        for (int nodo : cicli[i]) {
            cout << nodo << " -> ";
        }
        cout << cicli[i].front() << "\n";
    }


    cout << "\n=== MAGLIE FONDAMENTALI (METODO DFS) ===\n";
    vector<vector<int>> cicli_dfs = dfs_cicli(circuito, T, coalbero2);
    
    for (size_t i = 0; i < cicli_dfs.size(); ++i) {
        cout << "Maglia " << i + 1 << ": ";
        for (int nodo : cicli_dfs[i]) {
            cout << nodo << " -> ";
        }
        cout << cicli_dfs[i].front() << "\n";
    }

    // ----------------------------------------------------
    // TEST BASE DI CICLI MINIMI (DE PINA)
    // ----------------------------------------------------
    cout << "\n=== BASE DI CICLI MINIMI (METODO DE PINA) ===\n";
    
    // 1. Chiamo l'algoritmo
    vector<Eigen::VectorXi> cicli_depina = de_pina(circuito, T, coalbero2);
    
    // 2. Salvo l'insieme degli archi per poterli tradurre dall'indice 'i' del vettore
    auto archi_G = circuito.all_edges();

    for (size_t i = 0; i < cicli_depina.size(); ++i) 
    {
        cout << "Ciclo Minimo " << i + 1 << ": ";
        
        Eigen::VectorXi C = cicli_depina[i];
        
        // Creo un grafo vuoto apposito per esportare questo specifico ciclo
        unidirected_graph<int, double> grafo_ciclo;
        
        auto it = archi_G.begin();
        for (int j = 0; j < C.size(); ++j, ++it) 
        {
            // Se in quella posizione c'è 1, l'arco fa parte del ciclo!
            if (C(j) == 1) 
            {
                int u = it->from();
                int v = it->to();
                
                // Stampo l'arco a schermo
                cout << "(" << u << "-" << v << ") ";
                
                // Lo aggiungo al grafo da esportare
                grafo_ciclo.add_edge({u, v});
            }
        }
        cout << "\n";

        // Esporto il ciclo come file .dot individuale
        string filename = "depina_ciclo_" + to_string(i + 1) + ".dot";
        esporta_graphviz(grafo_ciclo, filename);
    }
    
    cout << "\nGenerazione dei file .dot completata!\n";
    cout << "Puoi visualizzarli da terminale con il comando (esempio):\n";
    cout << "dot -Tpng depina_ciclo_1.dot -o ciclo1.png\n\n";


    return 0;
}