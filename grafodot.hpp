#include <iostream>
#include <fstream>
#include <string>
#include "grafi.hpp"

// Funzione per esportare il grafo in formato Graphviz (DOT)
void esporta_graphviz(const unidirected_graph<int, double>& circuito, const std::string& nome_file_out) {
    std::ofstream out(nome_file_out);
    
    if (!out.is_open()) {
        std::cerr << "Errore: impossibile creare il file " << nome_file_out << std::endl;
        return;
    }

    // Intestazione del file DOT (usiamo 'graph' per grafi non diretti)
    out << "graph Circuito {\n";
    
    // Impostazioni estetiche per renderlo bello da vedere
    out << "    // Stile dei nodi e degli archi\n";
    out << "    node [shape=circle, style=filled, fillcolor=lightblue, fontname=\"Helvetica\"];\n";
    out << "    edge [fontname=\"Helvetica\", fontsize=10];\n\n";

    // Scorriamo tutti gli archi del circuito
    for (const auto& arco : circuito.all_edges()) {
        
        std::string nome = arco.get_name();
        double valore = circuito.get_peso(arco);
        
        // Scegliamo l'unità di misura in base alla prima lettera del nome
        std::string unita = (nome[0] == 'V') ? "V" : "Ohm";

        // Scriviamo il collegamento (es: 1 -- 2)
        out << "    " << arco.from() << " -- " << arco.to();
        
        // Aggiungiamo l'etichetta con nome e valore
        out << " [label=\"" << nome << "\\n" << valore << " " << unita << "\"";

        // Tocco di classe: coloriamo di rosso gli archi che sono stati invertiti!
        if (arco.is_inverted()) {
            out << ", color=\"red\", fontcolor=\"red\"";
        }

        out << "];\n";
    }

    // Chiudiamo il grafo
    out << "}\n";
    out.close();
    
    std::cout << "File Graphviz salvato con successo: " << nome_file_out << std::endl;
}