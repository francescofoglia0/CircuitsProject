#include <iostream>
#include <cstdlib>
#include <vector>
#include <Eigen/Dense>
#include "GC.hpp"
#include "randfiller.h"

using namespace std;

int get_rand_int(randfiller& rf, int min, int max) {
    vector<int> v(1);
    rf.fill(v, min, max);
    return v[0];
}
int main(void)
{
    randfiller rf;

    // test di robustezza su 100 sistemi lineari dinamici
    for(int k = 0; k < 100; k++)
    {
        int n = get_rand_int(rf, 5, 19); 

        // genero una matrice a casuale simmetrica e definita positiva (libreria Eigen)
        Eigen::MatrixXd M = Eigen::MatrixXd::Random(n, n);
        Eigen::MatrixXd A = M.transpose() * M; 
        
        // aggiungo valori positivi sulla diagonale cosi sono sicuro 
        // che la matrice sia simmetrica, definita positiva e non singolare
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