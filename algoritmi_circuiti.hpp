#pragma once
#include "grafi.hpp"
#include "visite.hpp"
#include "lifofifo.hpp"
#include <vector>
#include <Eigen/Dense>
#include <map>

//implemento dfs per i cicli
using namespace std;
bool findpath(const unidirected_graph<int,double>& T, int u, int v,set<int>& visited,vector<int>& path)
{
    
    visited.insert(u);
    path.push_back(u);

    if(u==v) return true;
    for(const auto& n : T.neighours(u))
    {   
        //se non trovo il vicino nel set di visitati
        if(visited.find(n) == visited.end())
        {
            if(findpath(T,n,v,visited,path))
            {
                return true;
            }
        }
    }
    path.pop_back();
    return false;
}


vector<vector<int>> dfs_cicli(const unidirected_graph<int,double>& G,const unidirected_graph<int,double>& T,const unidirected_graph<int,double>& coalbero)
{
    vector<vector<int>> lista_cicli;
    for(const auto& arco : coalbero.all_edges())
    {   
        int u = arco.from();
        int v = arco.to();
        //creo un set per tenere traccia di chi ho visitato e un vector in un cui salvo il ciclo 
        set<int> visited;
        vector<int> path;

        //findpath modifica il path
        if(findpath(T,u,v,visited,path))
        {   
            path.push_back(u);//inserisco il nodo iniziale alla fine in modo da usare il codice per risolvere il sistema
        //se findpath mi restituisce vero allora inserisco quel ciclo trovato nel vettore di cicli
            lista_cicli.push_back(path);
        }
    }

    return lista_cicli;
}


//utile per de pina
// restituisce il vettore dei nodi del cammino minimo da 'sorg' a 'dest'
std::vector<int> bfs_depina(const unidirected_graph<int,double>& G, int sorg, int dest)
{
    // implemento bfs: la faccio partire da un nodo (v+) e la interrompo appena arriva al nodo
    // corrispondente dell'altro segno (v-)
    fifo<int> coda; 
    
    std::set<int> reached;
    std::map<int, int> padri; // chiave = nodo, valore = nodo da cui sono arrivato. Serve per
                              // costruire il percorso a ritroso 

    reached.insert(sorg);
    coda.put(sorg);
    padri[sorg] = -1; // la sorgente non ha un padre

    bool trovato = false;

    while(!coda.empty())
    {
        int nodo = coda.get();

        // appena estraggo la destinazione mi fermo (risparmia tempo di calcolo)
        if (nodo == dest) {
            trovato = true;
            break; 
        }

        for(const auto& vicino : G.neighours(nodo))
        {
            if(reached.find(vicino) == reached.end())
            {
                reached.insert(vicino);
                padri[vicino] = nodo; // mi segno che per andare al 'vicino' sono passato da 'nodo'
                coda.put(vicino);
            }
        }
    }

    // ricostruisco il cammino a ritroso
    std::vector<int> cammino;
    if (!trovato) 
        return cammino; // se non trovo un percorso restituisco un vettore vuoto

    int corrente = dest;
    while(corrente != -1) 
    {
        cammino.push_back(corrente);
        corrente = padri[corrente];
    } 
    //ritorna un vettore di nodi costruito a ritroso (coerente per tutti i percorsi)
    return cammino; 
}

//implemento de pina
//estrae la base dei cicli minimi di un circuito 
vector<vector<int>> de_pina(const unidirected_graph<int,double>& G,const unidirected_graph<int,double>& T,const unidirected_graph<int,double>& coalbero)
{
    vector<vector<int>> B; //lista di cicli che ritorno
    //matrice S ha sulle righe il numero di archi nel circuito e sulle colonne i cicli da trovare
    Eigen::MatrixXi S = Eigen::MatrixXi::Zero(G.all_edges().size(),coalbero.all_edges().size());

    //invece di prendere il numero di nodi totali, prendo l'ultimo nodo (che sarà il piu grande)
    //e a questo ci aggiungo 1, in questo modo quando creo i nodi duplicati posso fare
    //nodo_originale+n senza avere nodi duplicati (ad esempio potrei avere questo caso:
    //ho i nodi 1 2 3 5 6 7 -> n = 6 e nodo duplicato di 1 sara 1+6 = 7-> errore c'è già)
    int n = *G.all_nodes().rbegin() +1;
    

    //costruisco il grafo G supporto che ha i vertici duplicati v+,v-, in realta ogni veritice è implementato come:
    //v+n dove n è il numero di vertici che ci sono in G, quindi se v+ è in pos [i] -> v- in pos [i+n]
    unidirected_graph<int,double> Gsupporto(G);
    for(const auto& vecchi : G.all_edges())
    {
        int u = vecchi.from();
        int v = vecchi.to();
        u = u+n;
        v = v+n;
        Gsupporto.add_edge({u,v});
    }

    //costruisco S in base agli archi del coalbero, all'iterazione k, Sk ha valore 1 in posizione
    //i dove i è l'indice dell'arco nel grafo vero (ordine lessicografico)
    size_t k = 0;
    for(const auto& archi: coalbero.all_edges())
    {   
        size_t i = G.edge_number(archi); //indice dell'arco in G
        S(i,k) = 1; //costruiamo S in base all'ordine lessicografico degli archi
        k++;
    }

    for(size_t j = 0; j<k; j++)
    {
        auto Sj = S.col(j); //estraggo colonna j della matrice S
        unidirected_graph<int,double> Gprimo(Gsupporto); //faccio una copia di Gsupporto

        auto archi_di_G = G.all_edges(); //salvo il set in una variabile in modo da non dare problemi con all.edges()
        auto it = archi_di_G.begin(); //usiamo un iteratore per accedere all'elemento i del set
        int u; 

        //se alla colonna Sj trovo che un arco ha valore 1, allora per quell'arco modifico
        //la forma del grafo G', in particolare rimuovo da G' (abbiamo implementato rm_edge)
        //l'arco in questione, e aggiungo gli archi incrociati (u+,v-; u-,v+) e l'arco nel grafo
        //duplicato (u-,v-);
        for(int i = 0;i<Sj.size();i++,it++) 
        {
            if(Sj(i) == 1)
            {
                unidirected_edge<int> arco = *it; //dereferenziamo l'iteratore per accedere all'elemento
                u = arco.from(); 
                int v = arco.to();
                Gprimo.rm_edge(arco);
                Gprimo.rm_edge({u+n,v+n});
                Gprimo.add_edge({u,v+n});
                Gprimo.add_edge({u+n,v});
            }
        }

        //ricerca del ciclo piu corto
        vector<int> migliore;
        size_t taglia;
        //lanciamo la BFS a ogni nodo del grafo originale per cercare il cammino minimo
        //che parte da un nodo u+ e arriva in u-
        for(const auto& a: G.all_nodes())
        {
            std::vector<int> risultato_bfs = bfs_depina(Gprimo,a,a+n);
            //se la bfs restituisce un risultato allora vedo se questo è il primo risultato 
            //oppure se è il piu corto finora
            if(!risultato_bfs.empty())
            {
                if(migliore.empty() || risultato_bfs.size()<taglia)
                {
                    migliore = risultato_bfs;
                    taglia = migliore.size();
                }
            }
        }

        //se non trova un ciclo (e.g grafo disconnesso) salta il prossimo ciclo
        if(migliore.empty()) continue; 

        //trasformo la sequenza di nodi in vettore Cj di 0 e 1 (lungo quanto gli archi in G)
        //che ha 1 in posizione i se l'arco in posizione i nel grafo originale, è presente nel 
        //ciclo minimo trovato
        Eigen::VectorXi Cj = Eigen::VectorXi::Zero(G.all_edges().size());
        //creiamo un vettore per salvare il ciclo come sequenza di nodi
        vector<int> ciclo_minimo(taglia);
        int u_orig;
        int v_orig;
        for(size_t i = 0; i<taglia-1; i++)
        {   
            //riportiamo i nodi alla loro forma non duplicata
            u_orig = migliore[i];
            v_orig = migliore[i+1];
            if(u_orig>=n)
            {
                u_orig -= n;
            }
            if(v_orig>=n)
            {
                v_orig -= n;
            }
            unidirected_edge<int> e = {u_orig,v_orig};
            int indice = G.edge_number(e); //trovo a quale riga corrisponde l'arco
            Cj(indice) = 1; //quell'arco trovato vale 1 nel ciclo Cj
            ciclo_minimo[i] = u_orig;
        }
        //inserisco l'ultimo elemento di migliore nel ciclo_minimo(il primo elemento del ciclo)
        ciclo_minimo[taglia-1] = v_orig;
        //inserisco il ciclo minimo nella Base 
        B.push_back(ciclo_minimo);
        

        //ci vogliamo assicurare che i cicli da cercare siano linearmente indipendenti
        for(size_t h =j+1; h<k; h++)
        {   
            //prodotto scalare modulo 2 tra il ciclo trovato e le colonne restanti di S
            int prodscal = Cj.dot(S.col(h))%2;
            //se non sono ortogonali applichiamo lo XOR per correggere la matrice S
            if(prodscal == 1)
            {
                for(int i = 0; i<S.col(h).size();i++)
                {
                    S(i,h) ^= Sj(i);
                }
            }
        }
    }
    return B;
}
