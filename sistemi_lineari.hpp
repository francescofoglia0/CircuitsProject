#pragma once
#include "grafi.hpp"
#include <Eigen/Dense>
#include <string>
#include <tuple> //per restituire 3 elementi



std::tuple<Eigen::MatrixXi,Eigen::MatrixXd,Eigen::VectorXd> creazione_B_R(const unidirected_graph<int,double>& circuito,const vector<Eigen::VectorXi>& C)
{
    int m = circuito.get_resistenze(); //numero di resistenze
    size_t n = C.size();
    Eigen::MatrixXi B = Eigen::MatrixXi::Zero(m,n);
    Eigen::MatrixXd R = Eigen::MatrixXd::Zero(m,m);
    //inizializziamo il vettore di generatori che come dimensione = numero di maglie
    //perche se una maglia ha piu generatori li sommiamo con segno
    Eigen::VectorXd v = Eigen::VectorXd::Zero(n); 
    for(size_t i = 0; i<n; i++)
    {
        double somma_generatore = 0;
            //sto confrontando con size di una matrice eigen (ha segno anche negativo)       
        for(int j = 0; j<C[i].size()-1; j++)
        {
            unidirected_edge<int> arco_copia = {C[i](j),C[i](j+1)};
            int indice_arco = circuito.edge_number(arco_copia);
            unidirected_edge<int> arco = circuito.edge_at(indice_arco);
            if(arco.get_name()[0] =='R') 
            {
                //per calcolare l'indice della resistenza, prendiamo qual è il secondo valore
                //della resistenza salvata nel grafo (e.g 'R10'->10)
                int indice_resistenza = std::stoi(arco.get_name().substr(1))-1;
                double valore = circuito.get_peso(arco);
                //se indice resistenza è 'R2' ->2 allora metto in R(2,2) = valore di R2
                R(indice_resistenza,indice_resistenza) = valore; //metto valore sulla diagonale di R
                if(C[i](j)<C[i](j+1))
                {
                    B(indice_resistenza,i) = 1;
                }
                else
                {
                    B(indice_resistenza,i) = -1;
                }
            }
            else if(arco.get_name()[0] == 'V')
            {
                double generatore = circuito.get_peso(arco);
                bool invertito = arco.is_inverted(); 
                //se sto scorrendo da piu piccolo a piu grande e il generatore non è stato invertito
                if(C[i](j)<C[i](j+1) && !invertito)
                {
                    generatore = -generatore;
                }
                //se sto scorrendo da piu grande a piu piccolo e il generatore è stato invertito
                else if(C[i](j)>C[i](j+1) && invertito)
                {
                    generatore = -generatore;
                }
                //negli altri casi il segno è gia giusto
                somma_generatore += generatore;
            }
        }
        v(i) = somma_generatore;
    }
    return {B,R,v};
}



