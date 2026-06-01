#include <iostream>
#include <cassert> 
#include <vector>
#include "grafi.hpp"
#include "visite.hpp"
#include "algoritmi_circuiti.hpp"
#include "lifofifo.hpp"

using namespace std;

// --- FUNZIONE HELPER PER I TEST ---
// Conta quanti archi (quanti '1') ci sono nel vettore del ciclo
int conta_archi(const Eigen::VectorXi& ciclo) {
    int count = 0;
    for(int i = 0; i < ciclo.size(); i++) {
        if(ciclo(i) == 1) count++;
    }
    return count;
}


// --- TEST 1: IL QUADRATO SEMPLICE ---
void test_grafo_quadrato() 
{
    cout << "Test 1: Grafo Quadrato (1 maglia, 4 archi)... ";
    
    unidirected_graph<int, double> circuito;
    circuito.add_edge({1, 2});
    circuito.add_edge({2, 3});
    circuito.add_edge({3, 4});
    circuito.add_edge({4, 1}); 
    
    unidirected_graph<int, double> T;
    T.add_edge({1, 2});
    T.add_edge({2, 3});
    T.add_edge({3, 4});

    unidirected_graph<int, double> coalbero;
    coalbero.add_edge({4, 1});

    vector<Eigen::VectorXi> cicli = de_pina(circuito, T, coalbero);

    assert(cicli.size() == 1); 
    assert(conta_archi(cicli[0]) == 4);

    cout << "PASSED!" << endl;
}


// --- TEST 2: L'ALBERO (ZERO CICLI) ---
void test_albero_lineare() 
{
    cout << "Test 2: Grafo ad Albero (0 maglie, nessuna chiusura)... ";
    
    unidirected_graph<int, double> circuito;
    circuito.add_edge({1, 2});
    circuito.add_edge({2, 3});
    circuito.add_edge({3, 4});
    // Non chiudo il circuito! È solo una linea.
    
    // L'albero di copertura è identico al circuito
    unidirected_graph<int, double> T = circuito;

    // Il coalbero è vuoto
    unidirected_graph<int, double> coalbero;

    vector<Eigen::VectorXi> cicli = de_pina(circuito, T, coalbero);

    // Mi aspetto esattamente 0 cicli
    assert(cicli.size() == 0); 

    cout << "PASSED!" << endl;
}


// --- TEST 3: GRAFO A DUE MAGLIE (QUADRATO + DIAGONALE) ---
void test_due_maglie_adiacenti() 
{
    cout << "Test 3: Quadrato con diagonale (2 maglie, ricerca del minimo)... ";
    
    unidirected_graph<int, double> circuito;
    circuito.add_edge({1, 2});
    circuito.add_edge({2, 3});
    circuito.add_edge({3, 4});
    circuito.add_edge({4, 1}); 
    circuito.add_edge({1, 3}); // La diagonale che taglia il quadrato in due triangoli
    
    // Albero di copertura (3 lati che toccano tutti e 4 i nodi)
    unidirected_graph<int, double> T;
    T.add_edge({1, 2});
    T.add_edge({2, 3});
    T.add_edge({3, 4});

    // Coalbero (i 2 lati rimasti)
    unidirected_graph<int, double> coalbero;
    coalbero.add_edge({4, 1});
    coalbero.add_edge({1, 3});

    vector<Eigen::VectorXi> cicli = de_pina(circuito, T, coalbero);

    // Mi aspetto esattamente 2 cicli (poiché ci sono 2 archi nel coalbero)
    assert(cicli.size() == 2); 
    
    // De Pina DEVE trovare i due triangoli (lunghezza 3 e 3) e NON il quadrato esterno (lunghezza 4)!
    assert(conta_archi(cicli[0]) == 3);
    assert(conta_archi(cicli[1]) == 3);

    cout << "PASSED!" << endl;
}


// --- TEST 4: GRAFO DISCONNESSO ---
void test_grafo_disconnesso() 
{
    cout << "Test 4: Grafo disconnesso (1 triangolo e 1 linea separata)... ";
    
    unidirected_graph<int, double> circuito;
    // Componente 1: Triangolo
    circuito.add_edge({1, 2});
    circuito.add_edge({2, 3});
    circuito.add_edge({3, 1});
    // Componente 2: Linea isolata
    circuito.add_edge({8, 9}); 
    
    // Foresta di copertura (Albero 1 + Albero 2)
    unidirected_graph<int, double> T;
    T.add_edge({1, 2});
    T.add_edge({2, 3});
    T.add_edge({8, 9});

    // Coalbero
    unidirected_graph<int, double> coalbero;
    coalbero.add_edge({3, 1});

    vector<Eigen::VectorXi> cicli = de_pina(circuito, T, coalbero);

    // Mi aspetto 1 solo ciclo (il triangolo)
    assert(cicli.size() == 1); 
    assert(conta_archi(cicli[0]) == 3);

    cout << "PASSED!" << endl;
}


int main() 
{
    cout << "\n=============================================" << endl;
    cout << "   INIZIO SUITE DI TEST (ALGORITMO DE PINA)  " << endl;
    cout << "=============================================\n" << endl;
    
    test_grafo_quadrato();
    test_albero_lineare();
    test_due_maglie_adiacenti();
    test_grafo_disconnesso();
    
    cout << "\n=============================================" << endl;
    cout << "      TUTTI I 4 TEST SUPERATI CON SUCCESSO!  " << endl;
    cout << "=============================================\n" << endl;
    
    return 0;
}