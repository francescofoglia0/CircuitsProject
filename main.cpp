#include <iostream>
#include <string>
#include "grafi.hpp" 
#include "parser.hpp" 
#include "grafodot.hpp"

using namespace std;

int main(int argc, const char *argv[])
{
    if (argc < 2)  // verifico che sia stato passato il parametro da terminale
    {
        cerr << "Non hai inserito il nome del file da leggere\n"; 
        return 1;
    }
    
    string nomefile = argv[1];

    // 1. Invochiamo il parser. 
    // Specifichiamo i template <int, double> per coerenza con la funzione leggi_circuito.
    unidirected_graph<int, double> circuito = leggi_circuito(nomefile);

    esporta_graphviz(circuito, "prova.dot");
    return 0;
}