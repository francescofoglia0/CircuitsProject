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
#include "calcolocond.hpp"

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

    //gestiamo il caso in cui il grafo passato sia vuoto o non ci siano resistenze
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
        
            // Uniamo gli archi e i nodi di questa componente al grafo T
            for(const auto& arco : albero.all_edges()) {
                T.add_edge(arco);
                nodi_visitati.insert(arco.from());
                nodi_visitati.insert(arco.to());
            }
        }
    }
    
    // il coalbero conterrà ora solo gli archi rimanenti di tutti i sottocicli
    unidirected_graph<int,double> coalbero = circuito - T;
    cout<<"--------------------------------------------------------------------------------\n";
    cout << "DETTAGLI TECNICI\n";
    cout<<"--------------------------------------------------------------------------------\n";
    //DFS
    // Faccio partire il cronometro
    auto start_dfs = chrono::high_resolution_clock::now();

    // Eseguo l'algoritmo puro
    vector<vector<int>> cicli_dfs = dfs_cicli(circuito, T, coalbero);
    
    // Fermo il cronometro
    auto end_dfs = chrono::high_resolution_clock::now();
    
    // Calcolo la durata in millisecondi
    chrono::duration<double, milli> durata_dfs = end_dfs - start_dfs;
    cout << "\nTempo di esecuzione DFS: " << durata_dfs.count() << " ms\n";
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


    // DEPINA
    auto start_depina = chrono::high_resolution_clock::now();
    
    vector<vector<int>> cicli_depina = de_pina(circuito, T, coalbero);
    
    auto end_depina = chrono::high_resolution_clock::now();
    
    chrono::duration<double, milli> durata_depina = end_depina - start_depina;
    cout << "\nTempo di esecuzione De Pina: " << durata_depina.count() << " ms\n";
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
    }

    //calcolo le matrici del sistema per il numero di condizionamento
    auto [B_dfs, R_dfs, v_dfs] = creazione_B_R(circuito, cicli_dfs);
    auto [B_depina, R_depina, v_depina] = creazione_B_R(circuito, cicli_depina);

    // Chiamata alla funzione del nuovo file .hpp
    double cond_dfs = calcola_condizionamento(B_dfs, R_dfs);
    double cond_depina = calcola_condizionamento(B_depina, R_depina);


    // Scegliamo di risolvere il circuito usando i cicli ottimali di De Pina
    // auto [B, R, v] = creazione_B_R(circuito, cicli_dfs);
    cout << "\nMatrici e vettori usando i cicli DFS:";
    cout << "\nMatrice di Incidenza Maglie (B):\n" << B_dfs << "\n\n";
    cout << "Matrice delle Resistenze (R):\n" << R_dfs << "\n\n";
    cout << "Vettore dei Generatori (V):\n" << v_dfs << "\n";
    cout << "\nCalcolo del numero di condizionamento del sistema (Matrice A = B^T * R * B)\n";
    cout << " k(A) con cicli DFS = " << cond_dfs << "\n";

    cout<<"\n--------------------------------------------------------------------------------\n";


    cout << "\nMatrici e vettori usando i cicli DEPINA:";
    cout << "\nMatrice di Incidenza Maglie (B):\n" << B_depina << "\n\n";
    cout << "Matrice delle Resistenze (R):\n" << R_depina << "\n\n";
    cout << "Vettore dei Generatori (V):\n" << v_depina << "\n";
    cout << "\nCalcolo del numero di condizionamento del sistema (Matrice A = B^T * R * B)\n";
    cout << " k(A) con cicli De Pina = " << cond_depina << "\n\n";
   
    // Soluzione del problmea tramite dfs
    auto sol_dp = chrono::high_resolution_clock::now();

    auto [I_rami, V_rami] = calcola_output(circuito, cicli_depina);

    auto fine_dp = chrono::high_resolution_clock::now();

    chrono::duration<double, milli> tempo_dp = fine_dp - sol_dp;
    cout<<"--------------------------------------------------------------------------------\n";
    cout << "OUTPUT RICHIESTO\n";
    cout<<"--------------------------------------------------------------------------------\n";
    cout << "\nTempo di costruzione matrici e risoluzione dei sistemi lineari tramite cicli DE PINA: " << tempo_dp.count() << " ms\n";
    cout << "Risultati considerando i cicli generati dall'algoritmo di de pina:\n";
    for(int i = 0; i < V_rami.size(); ++i)
    {
        double tensione = V_rami(i);
        double corrente = I_rami(i);
        
        // Ricostruiamo il nome interno che il parser ha assegnato (R1, R2...)
        string nome_interno = "R" + to_string(i + 1);
        
        // Traduciamo il nome interno nel nome originale della Netlist (es. R50)
        string nome_reale = mappa_nomi[nome_interno];
        
        cout << nome_reale << ": V = " << tensione << " volts, I = " << corrente << " amps\n";
    }
    cout<<"\n";

    //risolvo anche con i cicli dfs per vedere se cambia
    auto sol_dfs = chrono::high_resolution_clock::now();

    auto [i_maglie1, V_rami1] = calcola_output(circuito, cicli_dfs);

    auto fine_dfs = chrono::high_resolution_clock::now();
    
    chrono::duration<double, milli> tempo_dfs = fine_dfs - sol_dfs;

    cout << "\nTempo di costruzione matrici e risoluzione dei sistemi lineari tramite cicli DFS: " << tempo_dfs.count() << " ms\n";
    cout << "Risultati considerando i cicli generati dall'algoritmo dfs:\n";
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


    return 0;
}