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

    // cout << "\n=== MAGLIE FONDAMENTALI (METODO DFS) ===\n";
    // vector<vector<int>> cicli = dfs_cicli(circuito,T,coalbero2);
    
    // for (size_t i = 0; i < cicli.size(); ++i) {
    //     cout << "Maglia " << i + 1 << ": ";
    //     for (int nodo : cicli[i]) {
    //         cout << nodo << " -> ";
    //     }
    //     cout << cicli[i].front() << "\n";
    // }


    // cout << "\n=== MAGLIE FONDAMENTALI (METODO DFS) ===\n";
    // vector<vector<int>> cicli_dfs = dfs_cicli(circuito, T, coalbero2);
    
    // for (size_t i = 0; i < cicli_dfs.size(); ++i) {
    //     cout << "Maglia " << i + 1 << ": ";
    //     for (int nodo : cicli_dfs[i]) {
    //         cout << nodo << " -> ";
    //     }
    //     cout << cicli_dfs[i].front() << "\n";
    // }

    // // ----------------------------------------------------
    // // TEST BASE DI CICLI MINIMI (DE PINA)
    // // ----------------------------------------------------
    // cout << "\n=== BASE DI CICLI MINIMI (METODO DE PINA) ===\n";
    
    // // 1. Chiamo l'algoritmo
    // vector<Eigen::VectorXi> cicli_depina = de_pina(circuito, T, coalbero2);
    
    // // 2. Salvo l'insieme degli archi per poterli tradurre dall'indice 'i' del vettore
    // auto archi_G = circuito.all_edges();

    // for (size_t i = 0; i < cicli_depina.size(); ++i) 
    // {
    //     cout << "Ciclo Minimo " << i + 1 << ": ";
        
    //     Eigen::VectorXi C = cicli_depina[i];
        
    //     // Creo un grafo vuoto apposito per esportare questo specifico ciclo
    //     unidirected_graph<int, double> grafo_ciclo;
        
    //     auto it = archi_G.begin();
    //     for (int j = 0; j < C.size(); ++j, ++it) 
    //     {
    //         // Se in quella posizione c'è 1, l'arco fa parte del ciclo!
    //         if (C(j) == 1) 
    //         {
    //             int u = it->from();
    //             int v = it->to();
                
    //             // Stampo l'arco a schermo
    //             cout << "(" << u << "-" << v << ") ";
                
    //             // Lo aggiungo al grafo da esportare
    //             grafo_ciclo.add_edge({u, v});
    //         }
    //     }
    //     cout << "\n";

    //     // Esporto il ciclo come file .dot individuale
    //     string filename = "depina_ciclo_" + to_string(i + 1) + ".dot";
    //     esporta_graphviz(grafo_ciclo, filename);
    // }
    
    // cout << "\nGenerazione dei file .dot completata!\n";
    // cout << "Puoi visualizzarli da terminale con il comando (esempio):\n";
    // cout << "dot -Tpng depina_ciclo_1.dot -o ciclo1.png\n\n";



    // cout << "\n=== ESTRAZIONE MATRICI TOPOLOGICHE ED ELETTRICHE ===\n";
    
    // // Chiamo la funzione e spacchetto la tupla in 3 variabili
    // auto [B, R, V] = creazione_B_R(circuito, cicli_depina);

    // cout << "--- Matrice di Incidenza Maglie (B) ---\n";
    // cout << B << "\n\n";

    // cout << "--- Matrice delle Resistenze (R) ---\n";
    // cout << R << "\n\n";

    // cout << "--- Vettore dei Generatori (V) ---\n";
    // cout << V << "\n\n";

    // // ----------------------------------------------------
    // // RISOLUZIONE DEL CIRCUITO ELETTRICO (LEGGI DI KIRCHHOFF)
    // // ----------------------------------------------------
    // cout << "=== RISULTATI DEL CIRCUITO ===\n";

    // // 1. Cast di B a double (B è MatrixXi, R è MatrixXd, Eigen richiede tipi uguali per moltiplicare)
    // Eigen::MatrixXd Bd = B.cast<double>();

    // // 2. Costruzione della matrice del sistema: A = B^T * R * B
    // Eigen::MatrixXd A = Bd.transpose() * R * Bd;

    // // 3. Risoluzione del sistema lineare A * i_maglie = V 
    // // Usiamo colPivHouseholderQr() che è un solutore robusto ed efficiente per matrici quadrate
    // Eigen::VectorXd i_maglie = A.colPivHouseholderQr().solve(V);

    // // 4. Calcolo delle correnti su ogni singola resistenza: I_rami = B * i_maglie
    // Eigen::VectorXd I_rami = Bd * i_maglie;

    // // 5. Calcolo delle tensioni su ogni singola resistenza: V_rami = R * I_rami
    // Eigen::VectorXd V_rami = R * I_rami;

    // // 6. Stampa a schermo formattata
    // // Sappiamo che l'indice 0 corrisponde a R1, l'1 a R2, ecc.
    // for(int i = 0; i < V_rami.size(); ++i)
    // {
    //     // Uso un arrotondamento molto piccolo per evitare robe tipo 2.19999999999 dovute al double
    //     double tensione = round(V_rami(i) * 1000.0) / 1000.0;
    //     double corrente = round(I_rami(i) * 1000.0) / 1000.0;
        
    //     cout << "R" << (i + 1) << ": V = " << tensione << " volts, I = " << corrente << " amps.\n";
    // }


    // ----------------------------------------------------
    // RICERCA MAGLIE FONDAMENTALI (METODO DFS)
    // ----------------------------------------------------
    cout << "\n=== MAGLIE FONDAMENTALI (METODO DFS) ===\n";
    
    // 1. Chiamo il tuo algoritmo DFS
    vector<vector<int>> cicli_dfs = dfs_cicli(circuito, T, coalbero2);
    
    // 2. Converto i cicli da std::vector a Eigen::VectorXi per la funzione creazione_B_R
    vector<Eigen::VectorXi> cicli_eigen;
    for (const auto& ciclo_std : cicli_dfs) 
    {
        Eigen::VectorXi ciclo_eig(ciclo_std.size());
        for (size_t k = 0; k < ciclo_std.size(); ++k) 
        {
            ciclo_eig(k) = ciclo_std[k];
        }
        cicli_eigen.push_back(ciclo_eig);
    }

    // 3. Stampo le maglie e creo i file per Graphviz
    for (size_t i = 0; i < cicli_dfs.size(); ++i) 
    {
        cout << "Maglia DFS " << i + 1 << ": ";
        
        const auto& C = cicli_dfs[i];
        unidirected_graph<int, double> grafo_ciclo;
        
        for (size_t j = 0; j < C.size() - 1; ++j) 
        {
            cout << C[j] << " -> ";
            grafo_ciclo.add_edge({C[j], C[j+1]});
        }
        cout << C.back() << "\n";

        // Esporto il ciclo DFS
        string filename = "dfs_ciclo_" + to_string(i + 1) + ".dot";
        esporta_graphviz(grafo_ciclo, filename);
    }

    // ----------------------------------------------------
    // ESTRAZIONE MATRICI TOPOLOGICHE ED ELETTRICHE
    // ----------------------------------------------------
    cout << "\n=== ESTRAZIONE MATRICI TOPOLOGICHE ED ELETTRICHE ===\n";
    
    // Chiamo la funzione passando i cicli convertiti in formato Eigen
    auto [B, R, V] = creazione_B_R(circuito, cicli_eigen);

    cout << "--- Matrice di Incidenza Maglie (B) ---\n";
    cout << B << "\n\n";

    cout << "--- Matrice delle Resistenze (R) ---\n";
    cout << R << "\n\n";

    cout << "--- Vettore dei Generatori (V) ---\n";
    cout << V << "\n\n";

    // ----------------------------------------------------
    // RISOLUZIONE DEL CIRCUITO ELETTRICO (LEGGI DI KIRCHHOFF)
    // ----------------------------------------------------
    cout << "=== RISULTATI DEL CIRCUITO ===\n";

    // 1. Cast di B a double per poterla moltiplicare con R (che è di double)
    Eigen::MatrixXd Bd = B.cast<double>();

    // 2. Costruzione della matrice del sistema: A = B^T * R * B
    Eigen::MatrixXd A = Bd.transpose() * R * Bd;

    // 3. Risoluzione del sistema lineare A * i_maglie = V 
    Eigen::VectorXd i_maglie = A.colPivHouseholderQr().solve(V);

    // 4. Calcolo delle correnti di ramo: I_rami = B * i_maglie
    Eigen::VectorXd I_rami = Bd * i_maglie;

    // 5. Calcolo delle tensioni di ramo: V_rami = R * I_rami
    Eigen::VectorXd V_rami = R * I_rami;

    // 6. Stampa a schermo formattata
    for(int i = 0; i < V_rami.size(); ++i)
    {
        // Arrotondo a 3 cifre decimali per pulizia a schermo
        double tensione = round(V_rami(i) * 1000.0) / 1000.0;
        double corrente = round(I_rami(i) * 1000.0) / 1000.0;
        
        cout << "R" << (i + 1) << ": V = " << tensione << " V, I = " << corrente << " A\n";
    }

    return 0;
}