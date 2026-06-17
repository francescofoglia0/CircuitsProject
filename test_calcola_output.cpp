#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <set>
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
    // inizializzo i numeri casuali
    randfiller rf;
    string nome_file = "filetestout.txt";

    // test su 100 sistemi 
    for(int k = 0; k < 100; k++)
    {
        //scrivo file da passaer alla funzione
        ofstream out(nome_file);
        set<pair<int, int>> archi;
        int num_nodi = get_rand_int(rf, 5, 16);
        int num_archi = num_nodi + get_rand_int(rf, 0, 11);
        int id_comp = 1;
        
        for(int e = 0; e < num_archi; e++)
        {
            int u = get_rand_int(rf, 1, num_nodi);
            int v = get_rand_int(rf, 1, num_nodi);
            
            if(u == v || archi.count({min(u,v), max(u,v)})) continue;
            archi.insert({min(u,v), max(u,v)});
            
            // forzo la creazione di 1 solo generatore ('V') al primo arco, gli altri tutti 'R'.
            // impedisce le maglie in cortocircuito ma popola il vettore dei termini noti
            char tipo = (e == 0) ? 'V' : 'R';
            double val = get_rand_double(rf, 1.0, 50.0);
            out << tipo << id_comp++ << " " << val << " " << u << " " << v << "\n";
        }
        out.close();

        auto [circuito, mappa] = leggi_circuito(nome_file);
        if(circuito.get_resistenze() == 0) continue;

        unidirected_graph<int,double> T;
        set<int> nodi_visitati;
        
        // aggiunta la logica della foresta per evitare matrici singolari su reti disconnesse
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
        
        // estraggo le tensioni sui rami dal risolutore
        auto [i_rami, v_rami] = calcola_output(circuito, cicli);

        // legge di kirchhoff (B^t * v_rami = v_gen)
        Eigen::VectorXd kvl = B.transpose() * v_rami;
        
        if((kvl - v_gen).norm() > 1e-4)
        {
            cout << "Errore\n";
            return EXIT_FAILURE;
        }
    }
    remove(nome_file.c_str());
    cout << "Tutto giusto\n";
    return EXIT_SUCCESS;
}