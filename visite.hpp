#pragma once
#include "grafi.hpp"
#include "lifofifo.hpp"
#include <set>
#include <map>
#include <limits> //per infinito


template<typename T, typename C>
unidirected_graph<T> graph_visit(const unidirected_graph<T>& G,const T& sorg,C& cont)
{
    unidirected_graph<T> grafo_ris;
    std::set<T> reached;
    
    //controllo che il nodo sorgente passato, esista nel grafo
    auto nodes = G.all_nodes();
    if(nodes.find(sorg) == nodes.end()) //se l'iteratore è uguale a .end(), il nodo non è presente
        return grafo_ris;

    reached.insert(sorg);
    cont.put(sorg);

    while(!cont.empty())
    {
        //estraggo il nodo, se cont è queue(FIFO) usa BFS e .get() restituisce nodo più vecchio(ampiezza),
        //altrimenti se cont è stack(LIFO) usa DFS, .get() restuisce nodo più nuovo (profondità)
        T nodo = cont.get();

        //prendo i vicini del nodo 
        for(const auto& vicino : G.neighours(nodo))
        {
            //controllo se il vicino NON è gia stato visitato
            if(reached.find(vicino) == reached.end())
            {
                reached.insert(vicino);
                grafo_ris.add_edge({nodo,vicino});
                //contenitore posiziona il vicino o in cima (stack) o in fondo (queue)
                cont.put(vicino);
            }
        }
    }
    return grafo_ris;
}


//uso dsf fake come funzione che viene richiamata dalla ricorsiva, in modo da passare a
//def ricorsiva solo il grafo inizale e il nodo
template<typename T>
void dfs_fake(const unidirected_graph<T>& G, const T& nodo, std::set<T>& reached, unidirected_graph<T>& risultato)
{
    // Scorro i vicini nell'ordine fornito dal set dei vicini di nodo
    for (const auto& vicino : G.neighours(nodo))
    {
        // Se il vicino non è stato visitato, lo visito subito
        if (reached.find(vicino) == reached.end())
        {
            reached.insert(vicino);           // Lo segno come raggiunto
            risultato.add_edge({nodo, vicino});  // Aggiungo l'arco all'albero
            
        //Ricorsione: vado in profondità su questo ramo prima di guardare il prossimo vicino di nodo
            dfs_fake(G, vicino, reached, risultato);
        }
    }
}

template<typename T>
unidirected_graph<T> recursive_dfs(const unidirected_graph<T>& G, const T& nodos)
{
    unidirected_graph<T> alberoris;
    std::set<T> reached;
    //verifico che nodo sorgente esista
    auto nodi = G.all_nodes();
    if(nodi.find(nodos) != nodi.end())
    {   
        reached.insert(nodos);
        dfs_fake(G,nodos,reached,alberoris);
    }
    return alberoris;
}

                                //se non specificato usa double
template<typename T,typename P = double>
unidirected_graph<T,P> dijkstra(const unidirected_graph<T,P>& G, const T& sorgente)
{
    unidirected_graph<T,P> grafo_ris;
    //controllo che il nodo sorgente sia effettivamente nel grafo
    auto allnodi = G.all_nodes();
    if(allnodi.find(sorgente) == allnodi.end())
    {
        return grafo_ris; //ritorno un grafo vuoto se non è presente il nodo sorgente
    }
    //creo il valore infinito
    P infinito;

    if (std::numeric_limits<P>::has_infinity)
    {
        // se il tipo P supporta l'infinito matematico (per float o double)
        infinito = std::numeric_limits<P>::infinity(); 
    }
    else 
    {
        // se il tipo P NON lo supporta ( per int)
        infinito = std::numeric_limits<P>::max(); // Prendi il numero più grande possibile
    }
    std::map<T,P> dist;
    std::map<T,T> pred;

    //inizializzo il "vettore" delle distanze dello pseudocodice
    for (const auto& nodo : G.all_nodes()) {
        dist[nodo] = infinito;
    }
    
    dist[sorgente] = 0;

    priorityQ<T,P> PQ;
    //invece di inserire tutti i nodi nella PQ (non so se è sempre presente un arco tra due nodi)
    //inserisco invece solo la sorgente. I nodi verranno aggiunti solo se raggiungibili
    PQ.put(sorgente,0);
    while(!PQ.empty())
    {   
        //tolgo dalla coda nodo e peso corrente
        elementoPQ<T,P> corrente = PQ.get();
        //controllo se il nodo estratto ha un peso (distanza) che è maggiore della 
        //distanza minima gia trovata, in quel caso lo scarto e passo al prossimo
        if(corrente.peso>dist[corrente.nodo])
            continue;
        for (const auto& vicino : G.neighours(corrente.nodo))
        {
            P distanza = G.get_peso({corrente.nodo,vicino}); //W[nodo,vicino]
            P nuova_dist = dist[corrente.nodo] + distanza; //distanza da sorgente + W[nodo,vicino]

            if(dist[vicino]>nuova_dist)
            {
                dist[vicino] = nuova_dist;
                pred[vicino] = corrente.nodo;

            //PQ.DecreaseKey(vicino,dist[vicino]): aggiorno la priorità inserendo il nuovo nodo in PQ
                PQ.put(vicino,dist[vicino]);
            }
        }
    }

    //Ricostruisco il grafo dei cammini minimi
    //inserisco solo gli archi che rappresentano i cammini minimi per passare dal nodo sorgente
    //a tutti gli altri nodi
    for (const auto& [figlio, padre] : pred)
    {
        // Escludo la sorgente se associata a se stessa 
        if (figlio != sorgente) 
        {
            grafo_ris.add_edge({padre, figlio});
            grafo_ris.add_peso({padre, figlio}, G.get_peso({padre, figlio}));
        }
    }
    return grafo_ris;
}