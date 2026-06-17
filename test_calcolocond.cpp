#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <Eigen/Dense>
#include "calcolocond.hpp"

using namespace std;

int main(void)
{
    // inizializzo i numeri casuali
    srand(time(NULL));

    for(int k = 0; k < 100; k++)
    {
        int m = rand() % 10 + 5; 
        int n = rand() % 5 + 2;  

        // genero b e r casuali
        Eigen::MatrixXd B = Eigen::MatrixXd::Random(m, n);
        Eigen::MatrixXd R = Eigen::MatrixXd::Zero(m, m);

        // R deve essere diagonale e positiva per rispecchiare la fisica
        for(int i = 0; i < m; i++) {
            R(i, i) = (rand() % 100) + 1.0; 
        }

        double cond = calcola_condizionamento(B, R);
        
        // matematicamente, k(a) = max/min deve sempre essere >= 1
        // metto 0.999 per ammortizzare tolleranze infinitesime di arrotondamento float
        if(cond < 0.999 && cond != -1.0) {
            cout << "Errore: condizionamento minore di 1\n";
            return EXIT_FAILURE;
        }
    }

    // test mirato per intercettare le matrici singolari (divisione per zero)
    Eigen::MatrixXd B_sing = Eigen::MatrixXd::Zero(3, 3);
    Eigen::MatrixXd R_sing = Eigen::MatrixXd::Identity(3, 3);
    double cond_sing = calcola_condizionamento(B_sing, R_sing);
    
    // il nostro codice deve aver ritornato +infinito
    if(!std::isinf(cond_sing)) {
        cout << "Errore: matrice singolare non rilevata\n";
        return EXIT_FAILURE;
    }

    cout << "Tutto giusto\n";
    return EXIT_SUCCESS;
}