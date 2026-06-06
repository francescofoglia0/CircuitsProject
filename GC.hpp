#pragma once
#include <iostream>
#include <Eigen/Dense>

Eigen::VectorXd gcd(const Eigen::MatrixXd& A, const Eigen::VectorXd& b,
                    const Eigen::VectorXd& x0,double tol = 1e-6)
{
    if(A.rows() != A.cols())
    {
        std::cerr <<"ERRORE, la matrice deve essere quadrata";
        return x0;
    }
    // controlla anche che x0 non vuoto perche sarebbe diverso dalle righe di A
    if(A.rows() != b.size() || x0.size()!= A.rows())
    {
        std::cerr <<"ERRORE, dimensioni di A,B e x0 non compatibili";
        return x0;
    }

    double alpha, beta;
    // inizializzo x come x0
    Eigen::VectorXd x = x0;

    Eigen::VectorXd res = b-A*x;
    Eigen::VectorXd p = res;
    int iter = 0; 
    int itermax = 10 *b.size();
    // interrompo quando la norma del residuo è minore della tolleranza
    // controllo anche che le iterazioni siano minori di un certo itermax (in aritmetica non floating
    // point non ho per forza convergenza in n passi)
    while(res.norm()>tol && iter<itermax)
    {
        // creo il vettore definito come A dot p in modo da usarlo in alpha
        Eigen::VectorXd Ap = A*p;
        // dot fa p.transpose()*res come double
        alpha = (p.dot(res))/(p.dot(Ap));
        
        x = x+alpha*p;
        
        res = b-A*x;
        beta = (p.dot(A*res))/(p.dot(Ap));
        p = res-beta*p;
        iter++;
    }
    if (iter == itermax) {
        std::cerr << "Raggiunto il numero massimo di iterazioni (" << itermax << ") senza convergenza completa.\n";
    }
    return x;
}

