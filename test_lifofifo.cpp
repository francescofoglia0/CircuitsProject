#include <iostream>
#include <vector>
#include <cstdlib>
#include "lifofifo.hpp"
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
    randfiller rf;

    // eseguo 100 iterazioni di testing
    for(int k = 0; k < 100; k++)
    {
        int n_elementi = get_rand_int(rf, 10, 60);
        vector<int> inseriti;

        // testo la classe lifo (stack)
        lifo<int> pila;
        for(int i = 0; i < n_elementi; i++) {
            int val = get_rand_int(rf, 0, 99);
            inseriti.push_back(val);
            pila.put(val);
        }
        // verifico che i valori inseriti escano in ordine inverso
        for(int i = n_elementi - 1; i >= 0; i--) {
            if(pila.empty() || pila.get() != inseriti[i]) return EXIT_FAILURE;
        }
        if(!pila.empty()) return EXIT_FAILURE;

        // testo la classe fifo (queue)
        fifo<int> coda;
        for(int i = 0; i < n_elementi; i++) {
            coda.put(inseriti[i]);
        }
        // verifico che escano nello stesso identico ordine
        for(int i = 0; i < n_elementi; i++) {
            if(coda.empty() || coda.get() != inseriti[i]) return EXIT_FAILURE;
        }
        if(!coda.empty()) return EXIT_FAILURE;

        // testo la priority queue (min-heap)
        priorityQ<int, double> pq;
        for(int i = 0; i < n_elementi; i++) {
            double peso = get_rand_double(rf, 0.0, 99.9);
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