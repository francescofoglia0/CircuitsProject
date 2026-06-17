#include <iostream>
#include <cstdlib>
#include <ctime>
#include <Eigen/Dense>
#include "GC.hpp"

using namespace std;

int main(void)
{
    // inizializzo i numeri casuali
    srand(time(NULL));

    // test di robustezza su 100 sistemi lineari dinamici
    for(int k = 0; k < 100; k++)
    {
        int n = rand() % 15 + 5; 

        // genero una matrice a casuale simmetrica e definita positiva
        Eigen::MatrixXd M = Eigen::MatrixXd::Random(n, n);
        Eigen::MatrixXd A = M.transpose() * M; 
        
        // stabilizzo la matrice sommando una piccola identità per garantirne l'inversione
        A += Eigen::MatrixXd::Identity(n, n) * 0.1;

        // decido a priori quale deve essere la soluzione perfetta (x_true)
        Eigen::VectorXd x_true = Eigen::VectorXd::Random(n);
        
        // calcolo il termine noto b corrispondente
        Eigen::VectorXd b = A * x_true;
        Eigen::VectorXd x0 = Eigen::VectorXd::Zero(n);

        // do il sistema in pasto al mio gradiente coniugato
        Eigen::VectorXd x_calc = gcd(A, b, x0);

        // il residuo (distanza tra la soluzione calcolata e il vero b) deve essere basso
        Eigen::VectorXd residuo = A * x_calc - b;
        
        if(residuo.norm() > 1e-4) {
            cout << "Errore: il gradiente non converge\n";
            return EXIT_FAILURE;
        }
    }

    cout << "Tutto giusto\n";
    return EXIT_SUCCESS;
}