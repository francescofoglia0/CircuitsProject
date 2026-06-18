#pragma once
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>
#include <limits>
#include <cmath>
#include <iostream>

//in questo file abbiamo usato AI per farci indicare la migliore libreria e funzione di Eigen
// per calcolare gli autovalori di una matrice definita positiva
// in particolare abbiamo utilizzato la libreria:
//https://libeigen.gitlab.io/eigen/docs-nightly/classEigen_1_1SelfAdjointEigenSolver.html 

//  calcoliamo il numero di condizionamento in norma 2 di una matrice simmetrica
// definita positiva (A = B^T*R*B) usando il rapporto (|max autovalore|/|min autovalore|)

double calcola_condizionamento(const Eigen::MatrixXd& B, const Eigen::MatrixXd& R) 
{
    //matrice del sistema (A)
    Eigen::MatrixXd A = B.transpose() * R * B;
    
    //calcoliamo gli autovalori di A
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eigensolver(A);
    
    if (eigensolver.info() != Eigen::Success) {
        std::cerr << "ATTENZIONE: Impossibile calcolare gli autovalori della matrice A\n";
        return -1.0;
    }
    
    double min_eig = eigensolver.eigenvalues().minCoeff();
    double max_eig = eigensolver.eigenvalues().maxCoeff();
    
    // controlliamo per evitare la divisione per zero in caso di matrice singolare
    if (std::abs(min_eig) < 1e-12) {
        return std::numeric_limits<double>::infinity(); 
    }
    
    return std::abs(max_eig / min_eig);
}