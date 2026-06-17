#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "lifofifo.hpp"

using namespace std;

int main(void)
{
    // inizializzo i numeri casuali
    srand(time(NULL));

    // eseguo 100 iterazioni di testing
    for(int k = 0; k < 100; k++)
    {
        int n_elementi = rand() % 50 + 10;
        vector<int> inseriti;

        // test lifo (stack)
        lifo<int> pila;
        for(int i = 0; i < n_elementi; i++) {
            int val = rand() % 100;
            inseriti.push_back(val);
            pila.put(val);
        }
        // verifico che escano in ordine inverso
        for(int i = n_elementi - 1; i >= 0; i--) {
            if(pila.empty() || pila.get() != inseriti[i]) return EXIT_FAILURE;
        }
        if(!pila.empty()) return EXIT_FAILURE;

        // test fifo (queue)
        fifo<int> coda;
        for(int i = 0; i < n_elementi; i++) {
            coda.put(inseriti[i]);
        }
        // verifico che escano nello stesso identico ordine
        for(int i = 0; i < n_elementi; i++) {
            if(coda.empty() || coda.get() != inseriti[i]) return EXIT_FAILURE;
        }
        if(!coda.empty()) return EXIT_FAILURE;

        // test priority queue (min-heap)
        priorityQ<int, double> pq;
        for(int i = 0; i < n_elementi; i++) {
            double peso = (rand() % 1000) / 10.0;
            pq.put(i, peso); // il nodo è i, il peso è casuale
        }
        
        double peso_prec = -1.0;
        // estraggo e verifico che i pesi escano in ordine strettamente non decrescente
        while(!pq.empty()) {
            auto elem = pq.get();
            if(elem.peso < peso_prec) return EXIT_FAILURE; 
            peso_prec = elem.peso;
        }
    }
    
    cout << "Tutto giusto\n";
    return EXIT_SUCCESS;
}