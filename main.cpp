#include <iostream>
#include <string>
#include "grafi.hpp" 
#include "parser.hpp" 
#include "grafodot.hpp"
#include "visite.hpp"
#include "lifofifo.hpp"
#include "algoritmi_circuiti.hpp"
#include "sistemi_lineari.hpp"

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

    if(circuito.all_nodes().empty())
    {
        cerr<<"ERRORE: Il grafo passato è vuoto"<<endl;
        return 1;
    }
    if (circuito.get_resistenze() == 0) {
        cerr << "ERRORE: Il circuito non contiene resistenze. Non ha senso fisico\n";
        return 1;
    }

    // gestiamo il caso in cui è presente un nodo foglia in un circuito, ovvero un circuito è aperto
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

    // gestiamo qui il caso di grafo disconesso, i.e di piu componenti connesse nel grafo
    unidirected_graph<int,double> T; // Inizializziamo un grafo vuoto
    std::set<int> nodi_visitati; //qui salviamo tutti i nodi che visitiamo man mano

    // prendo ogni nodo del circuito per salvare tutte le componenti connesse
    for(int nodo : circuito.all_nodes())
    {
        // se troviamo un nodo non ancora coperto da un albero precedente
        if(nodi_visitati.find(nodo) == nodi_visitati.end())
        {
            // facciamo partire una nuova dfs per questa specifica sottorete
            lifo<int> pila_locale;
            unidirected_graph<int,double> albero = graph_visit(circuito, nodo, pila_locale);
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

    // DFS
    // eseguo l'algoritmo puro
    vector<vector<int>> cicli_dfs = dfs_cicli(circuito, T, coalbero);

    // scegliamo di risolvere il circuito usando i cicli dfs (piu veloce)
    auto [B, R, v] = creazione_B_R(circuito, cicli_dfs);


    // soluzione del circuito solo con dfs (risultato uguale a de pina, ma piu veloce)
    auto [i_maglie1, V_rami1] = calcola_output(circuito, cicli_dfs);
    cout <<"\n";
    for(int i = 0; i < V_rami1.size(); ++i)
    {
        // double tensione = round(V_rami1(i) * 1000.0) / 1000.0;
        // double corrente = round(i_maglie1(i) * 1000.0) / 1000.0;
        double tensione = V_rami1(i);
        double corrente = i_maglie1(i);
        
        // ricostruiamo il nome interno che il parser ha assegnato (R1, R2...)
        string nome_interno = "R" + to_string(i + 1);
        
        // traduciamo il nome interno nel nome originale 
        string nome_reale1 = mappa_nomi[nome_interno];
        
        cout << nome_reale1 << ": V = " << tensione << " volts, I = " << corrente << " amps.\n";
    }
    cout << "\n";
    return 0;
}