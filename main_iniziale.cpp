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

    auto [circuito,mappa_nomi] = leggi_circuito(nomefile);
    esporta_graphviz(circuito, "prova.dot");

    lifo<int> pila;
    unidirected_graph<int,double> albdfs = graph_visit(circuito,1,pila);
    esporta_graphviz(albdfs,"dfs.dot");

    unidirected_graph<int,double> coalbero2 = circuito-albdfs;
    esporta_graphviz(coalbero2,"coalbero.dot");

    if(circuito.all_nodes().empty())
    {
        cerr<<"ERRORE: Il grafo passato è vuoto"<<endl;
        return 1;
    }
    if (circuito.get_resistenze() == 0) {
        cerr << "ERRORE: Il circuito non contiene resistenze. Non ha senso fisico\n";
        return 1;
    }

    //gestiamo il caso in cui è presente un nodo foglia in un circuito, ovvero un circuito è aperto
    bool anomalie_topologiche = false;
    
    // un circuito chiuso deve avere tutti i nodi con almeno 2 connessioni.
    // se un nodo ha 1 solo vicino, è un nodo foglia allora il circuito è aperto.
    for(int nodo : circuito.all_nodes())
    {
        if(circuito.neighours(nodo).size() == 1)
        {
            cerr << "ATTENZIONE: Il Nodo " << nodo << " è un nodo foglia.\n";
            anomalie_topologiche = true;
        }
    }
    if(anomalie_topologiche)
    {
        cerr << "ERRORE: Rilevati circuiti aperti o componenti isolati. \n";
        // La simulazione prosegue, ma su questi rami la corrente sara' 0 amps.\n\n";
        return 1;
    }

    //gestiamo qui il caso di grafo disconesso, i.e di piu componenti connesse nel grafo
    unidirected_graph<int,double> T; // Inizializziamo un grafo vuoto
    std::set<int> nodi_visitati; //qui salviamo tutti i nodi che visitiamo man mano

    // Prendo ogni nodo del circuito per salvare tutte le componenti connesse
    for(int nodo : circuito.all_nodes())
    {
        // Se troviamo un nodo non ancora coperto da un albero precedente
        if(nodi_visitati.find(nodo) == nodi_visitati.end())
        {
            // facciamo partire una nuova dfs per questa specifica sottorete
            lifo<int> pila_locale;
            unidirected_graph<int,double> albero = graph_visit(circuito, nodo, pila_locale);
            
            //qui gestiamo il caso in cui un sottocircuito sia formato da soli generatori
            // std::set<int> nodi_compconn = albero.all_nodes();
            // bool ha_resistenza = false;
            // // scansioniamo tutti gli archi del circuito per vedere se c'è una R in questo sottocircuito
            // for (const auto& arco : circuito.all_edges()) {
            //     // se troviamo una resistenza usciamo
            //     if (nodi_compconn.find(arco.from()) != nodi_compconn.end()) {
            //         if (arco.get_name()[0] == 'R') {
            //             ha_resistenza = true;
            //             break;
            //         }
            //     }
            // }
            
            // if (!ha_resistenza) {
            //     cerr << "ERRORE: Trovato un sottocircuito formato solo da generatori. Non ha senso fisico." << endl;
            //     return 1;
            // }


            // Uniamo gli archi e i nodi di questa componente al grafo T
            for(const auto& arco : albero.all_edges()) {
                T.add_edge(arco);
                nodi_visitati.insert(arco.from());
                nodi_visitati.insert(arco.to());
            }
        }
    }
    
    // il coalbero conterrà ora solo le vere corde (archi rimanenti) di tutti i sottocicli
    unidirected_graph<int,double> coalbero = circuito - T;

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
    
    auto [I_rami, V_rami] = calcola_output(circuito, cicli_depina);
    
    for(int i = 0; i < V_rami.size(); ++i)
    {
        double tensione = round(V_rami(i) * 1000.0) / 1000.0;
        double corrente = round(I_rami(i) * 1000.0) / 1000.0;
        
        // Ricostruiamo il nome interno che il parser ha assegnato (R1, R2...)
        string nome_interno = "R" + to_string(i + 1);
        
        // Traduciamo il nome interno nel nome originale della Netlist (es. R50)
        string nome_reale = mappa_nomi[nome_interno];
        
        cout << nome_reale << ": V = " << tensione << " V, I = " << corrente << " A\n";
    }
    cout<<"\n\n";


    auto [i_maglie1, V_rami1] = calcola_output(circuito, cicli_dfs);
    
    for(int i = 0; i < V_rami1.size(); ++i)
    {
        double tensione = round(V_rami1(i) * 1000.0) / 1000.0;
        double corrente = round(i_maglie1(i) * 1000.0) / 1000.0;
        
        // Ricostruiamo il nome interno che il parser ha assegnato (R1, R2...)
        string nome_interno = "R" + to_string(i + 1);
        
        // Traduciamo il nome interno nel nome originale della Netlist (es. R50)
        string nome_reale1 = mappa_nomi[nome_interno];
        
        cout << nome_reale1 << ": V = " << tensione << " V, I = " << corrente << " A\n";
    }

    return 0;
}