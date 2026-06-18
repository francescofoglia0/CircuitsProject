#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <set>
#include <Eigen/Dense>
#include "parser.hpp"
#include "visite.hpp"
#include "algoritmi_circuiti.hpp"
#include "sistemi_lineari.hpp"
#include "randfiller.h"

using namespace std;

int get_rand_int(randfiller& rf, int min, int max) {
    vector<int> v(1);
    rf.fill(v, min, max);
    return v[0];
}

double get_rand_double(randfiller& rf, double min, double max) {
    vector<double> v(1);
    rf.fill(v, min, max);
    return v[0];
}

int main(void)
{
    // inizializzo il generatore casuale
    randfiller rf;
    // dobbiamo necesseriamente scrivere prima su un file di testo per leggere il circuito
    // e poi creare le strutture dati
    string nome_file = "filetestbr.txt";

    // ciclo di test su 100 circuiti
    for(int k = 0; k < 100; k++)
    {
        ofstream out(nome_file);
        set<pair<int, int>> archi;
        //prendiamo un numero di archi e nodi random
        int num_nodi = get_rand_int(rf, 5, 14);
        int num_archi = num_nodi + get_rand_int(rf, 0, 9);
        int id_comp = 1;
        
        for(int e = 0; e < num_archi; e++)
        {
            int u = get_rand_int(rf, 1, num_nodi);
            int v = get_rand_int(rf, 1, num_nodi);
            //se gli archi sono uguali oppure è gia presente quell'arco nella nostra struttura
            // dati (i.e {miminimo valore tra i due nodi, massimo tra i due nodi})
            if(u == v || archi.count({min(u,v), max(u,v)})) continue;
            archi.insert({min(u,v), max(u,v)});
            
            // uso solo resistenze per evitare che il test fallisca su maglie di soli generatori
            // (che in ogni caso è gia bloccato dalle nostre funzioni che gestiscono le eccezioni) 
            // che generebbero quindi cortocircuiti.
            //testiamo solo se la matrice R sia ben definita
            char tipo = 'R'; 
            double val = get_rand_double(rf, 1.0, 100.0);
            out << tipo << id_comp++ << " " << val << " " << u << " " << v << "\n";
        }
        out.close();

        //ora leggiamo il circuito e se non abbiamo resistenze in caso molto sfortunato saltiamo
        //alla prosssima iterazione
        auto [circuito, mappa] = leggi_circuito(nome_file);
        if(circuito.get_resistenze() == 0) continue;

        unidirected_graph<int,double> T;
        set<int> nodi_visitati;
        
        // costuisco l'albero di copertura con la dfs
        for(int nodo : circuito.all_nodes()) {
            if(nodi_visitati.find(nodo) == nodi_visitati.end()) {
                lifo<int> pila;
                //se non ho visitato il nodo costruisco l'albero dfs
                auto albero = graph_visit(circuito, nodo, pila);
                //da qui costruisco l'albero T
                for(const auto& a : albero.all_edges()) {
                    T.add_edge(a);
                    nodi_visitati.insert(a.from());
                    nodi_visitati.insert(a.to());
                }
            }
        }
        //prendo il coalbero da passare dalla dfs_cicli
        auto coalbero = circuito - T;
        auto cicli = dfs_cicli(circuito, T, coalbero);
        //se non ho cicli (caso molto sfortunato) salto iterazione
        if(cicli.empty()) continue;

        //creo le matrici da testare
        auto [B, R, v_gen] = creazione_B_R(circuito, cicli);

        int m = circuito.get_resistenze();
        int n = cicli.size();
        //controllo che sia mxn e che R sia mxm e che il vettore generato sia di dim n
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
                //  controllo  che fuori dalla diagonale ci siano solo zeri
                if(i != j && R(i, j) != 0) return EXIT_FAILURE;
            }
        }
        //verifico che la matrice B abbia solo numeri 0 -1 1
        for(int i = 0; i < m; i++)
        {
            for(int j = 0; j < n; j++)
            {
                double val = B(i,j);
                if(val != 1.0 && val != -1.0 && val != 0.0) return EXIT_FAILURE;
            }
        }
    }
    //eliminiamo il file creato
    remove(nome_file.c_str());
    cout << "Tutto giusto\n";
    return EXIT_SUCCESS;
}