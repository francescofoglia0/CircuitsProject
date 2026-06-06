#include <iostream>
#include <string>
#include "grafi.hpp" 
#include "parser.hpp" 
#include "grafodot.hpp"
#include "visite.hpp"
#include "lifofifo.hpp"
#include "algoritmi_circuiti.hpp"
#include "sistemi_lineari.hpp"
#include <chrono>

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

    unidirected_graph<int,double> coalbero2 = circuito-albdfs;
    esporta_graphviz(coalbero2,"coalbero.dot");

    if(circuito.all_nodes().empty())
    {
        cerr<<"Il grafo passato è vuoto"<<endl;
        return 1;
    }
    int sorgente = *(circuito.all_nodes().begin());
    lifo<int> altrapila;
    unidirected_graph<int,double> T = graph_visit(circuito,sorgente,altrapila);
    unidirected_graph<int,double> coalbero = circuito-T;

    // ====================================================================
    // 2. RICERCA MAGLIE FONDAMENTALI (DFS)
    // ====================================================================
    cout << "\n=== MAGLIE FONDAMENTALI (METODO DFS) ===\n";
    
    // Faccio partire il cronometro
    auto start_dfs = chrono::high_resolution_clock::now();
    
    // Eseguo l'algoritmo puro
    vector<vector<int>> cicli_dfs = dfs_cicli(circuito, T, coalbero);
    
    // Fermo il cronometro
    auto end_dfs = chrono::high_resolution_clock::now();
    
    // Calcolo la durata in millisecondi
    chrono::duration<double, milli> durata_dfs = end_dfs - start_dfs;
    cout << "--> Tempo di esecuzione DFS: " << durata_dfs.count() << " ms\n\n";
    for (size_t i = 0; i < cicli_dfs.size(); ++i) 
    {
        cout << "Maglia DFS " << i + 1 << ": ";
        const auto& C = cicli_dfs[i];
        for (size_t j = 0; j < C.size() - 1; ++j) 
        {
            cout << C[j] << " -> ";
        }
        cout << C.back() << "\n";
    }


    // ====================================================================
    // 3. BASE DI CICLI MINIMI (METODO DE PINA) E GRAPHVIZ
    // ====================================================================
    cout << "\n=== BASE DI CICLI MINIMI (METODO DE PINA) ===\n";
    
    auto start_depina = chrono::high_resolution_clock::now();
    
    vector<vector<int>> cicli_depina = de_pina(circuito, T, coalbero);
    
    auto end_depina = chrono::high_resolution_clock::now();
    
    chrono::duration<double, milli> durata_depina = end_depina - start_depina;
    cout << "--> Tempo di esecuzione De Pina: " << durata_depina.count() << " ms\n\n";
    for (size_t i = 0; i < cicli_depina.size(); ++i) 
    {
        cout << "Ciclo Minimo " << i + 1 << ": ";
        const auto& C = cicli_depina[i];
        unidirected_graph<int, double> grafo_ciclo;
        
        for (size_t j = 0; j < C.size() - 1; ++j) 
        {
            cout << C[j] << " -> ";
            grafo_ciclo.add_edge({C[j], C[j+1]});
        }
        cout << C.back() << "\n";

        // Esportiamo solo i cicli di De Pina in .dot
        string filename = "depina_ciclo_" + to_string(i + 1) + ".dot";
        esporta_graphviz(grafo_ciclo, filename);
    }
    cout << "Generazione dei file .dot completata.\n";


    // ====================================================================
    // 4. ESTRAZIONE MATRICI E VETTORI 
    // ====================================================================
    // Scegliamo di risolvere il circuito usando i cicli ottimali di De Pina
    cout << "\n=== ESTRAZIONE MATRICI TOPOLOGICHE ED ELETTRICHE ===\n";
    auto [B, R, v] = creazione_B_R(circuito, cicli_depina);

    cout << "--- Matrice di Incidenza Maglie (B) ---\n" << B << "\n\n";
    cout << "--- Matrice delle Resistenze (R) ---\n" << R << "\n\n";
    cout << "--- Vettore dei Generatori (V) ---\n" << v << "\n\n";


    // ====================================================================
    // 5. RISOLUZIONE DEL CIRCUITO ELETTRICO (LEGGI DI KIRCHHOFF)
    // ====================================================================
    cout << "=== RISULTATI DEL CIRCUITO ===\n";
    
    // Richiamiamo il modulo che usa il Gradiente Coniugato
    auto [i_maglie, V_rami] = calcola_output(circuito, cicli_depina);
    
    // Calcoliamo anche le correnti fisiche di ramo (I_rami = B * i_maglie)

    // Stampiamo a schermo in modo formattato e leggibile
    for(int i = 0; i < V_rami.size(); ++i)
    {
        // Arrotondiamo a 3 cifre decimali per pulire gli errori floating point
        double tensione = round(V_rami(i) * 1000.0) / 1000.0;
        double corrente = round(i_maglie(i) * 1000.0) / 1000.0;
        
        cout << "R" << (i + 1) << ": V = " << tensione << " V, I = " << corrente << " A\n";
    }
    cout<<"\n\n";


    auto [i_maglie1, V_rami1] = calcola_output(circuito, cicli_dfs);
    
    // Calcoliamo anche le correnti fisiche di ramo (I_rami = B * i_maglie)

    // Stampiamo a schermo in modo formattato e leggibile
    for(int i = 0; i < V_rami1.size(); ++i)
    {
        // Arrotondiamo a 3 cifre decimali per pulire gli errori floating point
        double tensione = round(V_rami1(i) * 1000.0) / 1000.0;
        double corrente = round(i_maglie1(i) * 1000.0) / 1000.0;
        
        cout << "R" << (i + 1) << ": V = " << tensione << " V, I = " << corrente << " A\n";
    }

    return 0;
}