#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "grafi.hpp" 
#include <map>
#include <tuple>
#include <string>
#include <vector>
#include <algorithm>

// la funzione riceve in input la stringa con il nome del file
std::pair<unidirected_graph<int, double>,std::map<std::string,std::string>> leggi_circuito(const std::string& nome_file) {
    unidirected_graph<int, double> circuito;
    std::map<std::string,std::string> mappa_nomi; //utile per salvare il numero progressivo di ogni resistenza
    
    // apriamo il file
    std::ifstream file(nome_file);
    
    // controlliamo se il file è stato aperto correttamente
    if (file.is_open()) {
        std::string nome_comp;
        double valore;
        int nodo1, nodo2;
        std::set<std::string> nomi_visti; // per bloccare un componente inserito due volte
        
        std::vector<std::tuple<std::string,double,int,int>> componenti;
       //leggiamo una stringa alla volta
        while (file >> nome_comp) {
            
            //se non viene passata R o V, ma un altro char diamo errore
            if (nome_comp[0] != 'R' && nome_comp[0] != 'V')
            {
            std::cerr << "ERRORE: Componente sconosciuto '" << nome_comp << "'. Devi inserire R o V.\n";
            exit(1);
            }
            //gestiamo il caso in cui dopo R o V non venga passato un numero
            for (size_t i = 1; i < nome_comp.length(); i++) 
            {
                if (!std::isdigit(nome_comp[i])) 
                {
                std::cerr << "ERRORE: Il componente '" << nome_comp << "' non e' valido. Dopo '"<< nome_comp[0]<< "' devi inserire un numero.\n";
                exit(1);
                }
            }

            //se c'è un duplicato do errore
            if (nomi_visti.find(nome_comp) != nomi_visti.end()) 
            {
            std::cerr << "ERRORE: Componente duplicato. '" << nome_comp << "' esiste già.\n";
            exit(1);
            }
            //estrazione dei numeri
            // se c'è una lettera al posto di un numero, o il file finisce a metà riga, file >> fallisce.
            if (!(file >> valore >> nodo1 >> nodo2)) 
            {
                std::cerr << "ERRORE: Parametri mancanti o non numerici per il componente " << nome_comp << ".\n";
                exit(1);
            }

            // controlliamo che il valore della resistenza non sia negativo (già gestito dall'ordine dell arco)
            if (nome_comp[0] == 'R' && valore <= 0) {
                std::cerr << "ERRORE: La resistenza " << nome_comp << " non può essere nulla o negativa (" << valore << " Ohm).\n";
                exit(1);
            }

            // controlliamo che non ci sono nodi che hanno un auto arco
            if (nodo1 == nodo2) {
                std::cerr << "ERRORE: Il componente " << nome_comp << " è collegato su se stesso (Nodo " << nodo1 << " -> Nodo " << nodo2 << ").\n";
                exit(1);
            }

            // se arriviamo qui, i dati sono corretti
            // creiamo l'arco passandogli i nodi e il nome (se è invertito verrà controllato
            // automaticamente nella classe edge)
            // unidirected_edge<int> arco(nodo1, nodo2, nome_comp);
            // circuito.add_edge(arco);
            // circuito.add_peso(arco, valore);
            
            // // Memorizziamo il nome per evitare duplicati futuri
            nomi_visti.insert(nome_comp);

            componenti.push_back({nome_comp,valore,nodo1,nodo2});
        }
        file.close();
        std::sort(componenti.begin(),componenti.end(),[](const std::tuple<std::string,double,int,int>& a,const std::tuple<std::string,double,int,int>&b)
        {
            std::string nome_a = get<0>(a);
            std::string nome_b = get<0>(b);
            if(nome_a[0] != nome_b[0])
            {
                return nome_a[0] < nome_b[0];
            }
            std::string numero_a = nome_a.substr(1);
            std::string numero_b = nome_b.substr(1);
            return std::stoi(numero_a)<std::stoi(numero_b);
        });
        int contatore_R = 1;
        int contatore_V = 1;

        for(const auto& c: componenti)
        {
            std::string nome_netlist = get<0>(c);
            double peso_arco = get<1>(c);
            int n1 = get<2>(c);
            int n2 = get<3>(c);

            std::string nome_interno;
            if(nome_netlist[0]== 'R')
            {
                nome_interno ='R'+ std::to_string(contatore_R);
                contatore_R++;
            }
            else if (nome_netlist[0] == 'V')
            {
                nome_interno = 'V'+std::to_string(contatore_V);
                contatore_V++;
            }
            mappa_nomi[nome_interno] = nome_netlist;
            unidirected_edge<int> arco(n1,n2,nome_interno);

            //per gestire i componenti in parallelo vediamo se edge number trova
            //l'arco a una posizione diversa da -1 -> vuol dire che è gia presente
            if(circuito.edge_number(arco)!=-1)
            {
                std::cerr << "ERRORE: Stai aggiungendo una componente in parallelo.\n";
                std::cerr << "Nell'arco"<< arco << "è già presente un altro componente\n";
                exit(1);
            }
            circuito.add_edge(arco);
            circuito.add_peso(arco,peso_arco);
        }
    }
    else {
        // Se il file non esiste, stampiamo un errore
        std::cerr << "ERRORE: impossibile aprire il file " << nome_file << std::endl;
        exit(1);
    }
    
    return {circuito,mappa_nomi};
}