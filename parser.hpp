#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "grafi.hpp" 

// La funzione riceve in input la stringa con il nome del file
unidirected_graph<int, double> leggi_circuito(const std::string& nome_file) {
    unidirected_graph<int, double> circuito;
    
    // Apriamo il file
    std::ifstream file(nome_file);
    
    // Controllo se il file è stato aperto correttamente
    if (file.is_open()) {
        
       //finche il file non è finito
        while (!file.eof()) {
            
            // Dichiariamo le variabili esatte del nostro circuito
            std::string nome_comp;
            double valore;
            int nodo1, nodo2;

            file >> nome_comp >> valore >> nodo1 >> nodo2;

            // Se siamo alla fine del file la stringa 'nome_comp'è vuota
            if (nome_comp == "") {
                break;
            }

            // Creiamo l'arco passandogli i nodi e il nome (se è invertito verrà controllato
            // automaticamente nella classe edge)
            unidirected_edge<int> arco(nodo1, nodo2, nome_comp);

            // Aggiungiamo l'arco e il suo valore (peso) al grafo
            circuito.add_edge(arco);
            circuito.add_peso(arco, valore);
        }
        
        file.close();
    }
    else {
        // Se il file non esiste, stampiamo un errore
        std::cerr << "Errore, impossibile aprire il file " << nome_file << std::endl;
    }
    
    // Restituiamo il grafo costruito
    return circuito;
}