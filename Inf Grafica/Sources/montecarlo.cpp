/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet 681721
 *
 * Clase que representa a montecarlo
 */
#include <iostream>
#include <cmath>
#include "montecarlo.hpp"
#include "objeto_cartesiano.hpp"
#include "punto.hpp"
#include "vector.hpp"
#include "operadorVector.hpp"
#include "matriz.hpp"
#include <vector>
#include <stdlib.h> 
#include <algorithm>
using namespace std;


void Montecarlo::set_values(double i,double a,Vector px, Vector normal, int numRayos)
{
    inclination=i;
    azimuth = a;
    n = normal;
    x = px;
    num = numRayos;
}
double Montecarlo::getInclination(){
    return inclination;
}
double Montecarlo::getAzimuth(){
    return azimuth;
}
Vector Montecarlo::getn(){
    return n;
}
Vector Montecarlo::getx(){
    return x;
}
int Montecarlo::getNum(){
    return num;
}

list<Matriz> Montecarlo::calcularT(){
    
    Vector azar;
    list<Vector> usados;
    list<Matriz> matT;
    srand(0);
    
    while(usados.size()<getNum()){
        azar.set_values(rand(),rand(),rand());
        
        while((azar.getX()==0 && azar.getY()==0 && azar.getZ()==0)){
            azar.set_values(rand(),rand(),rand());
        }
        usados.push_back(azar);
        
        
        Vector n= getn();
        Vector x = getx();
        
        Vector u = productoVectorial(azar,n);
        u.normalizar();
        Vector v = productoVectorial(n,u);
        v.normalizar();
        
        Matriz T;
        T.set_valuesColum(u,v,n,x);
         
        matT.push_back(T);
    }
    return matT;
}
/*
 * devuelve el determinante de la matriz 4x4 T 
 */
double Montecarlo::determinante(Matriz T){
    double t1 = T.get(2,1)*T.get(3,2)*T.get(4,4) + T.get(3,1)*T.get(4,2)*T.get(2,4) + T.get(2,2)*T.get(3,4)*T.get(4,1) - T.get(4,1)*T.get(3,2)*T.get(2,4) - T.get(4,2)*T.get(3,4)*T.get(2,1) - T.get(3,1)*T.get(2,2)*T.get(4,4);
    double t2 = T.get(1,1)*T.get(3,2)*T.get(4,4) + T.get(1,2)*T.get(3,4)*T.get(4,1) + T.get(3,1)*T.get(4,2)*T.get(1,4) - T.get(4,1)*T.get(3,2)*T.get(1,4) - T.get(4,2)*T.get(3,4)*T.get(1,1) - T.get(1,2)*T.get(3,1)*T.get(4,4);
    double t3 = T.get(1,1)*T.get(2,2)*T.get(4,4) + T.get(2,1)*T.get(4,2)*T.get(1,4) + T.get(1,2)*T.get(2,4)*T.get(4,1) - T.get(4,1)*T.get(2,2)*T.get(1,4) - T.get(4,2)*T.get(2,4)*T.get(1,1) - T.get(2,1)*T.get(1,2)*T.get(4,4);
    double t4 = T.get(1,1)*T.get(2,2)*T.get(3,4) + T.get(2,1)*T.get(3,2)*T.get(1,4) + T.get(1,2)*T.get(2,4)*T.get(3,1) - T.get(3,1)*T.get(2,2)*T.get(1,4) - T.get(3,2)*T.get(2,4)*T.get(1,1) - T.get(2,1)*T.get(1,2)*T.get(3,4); 
    
    return (T.get(1,3)*t1 - T.get(2,3)*t2 + T.get(3,3)*t3 - T.get(4,3)*t4);
    
}
Matriz Montecarlo::inversaT(Matriz T){
    double det = determinante(T);
    
    Matriz nueva;
    Vector u;
    
    u.set_X((T.get(2,2)*T.get(3,3)*T.get(4,4) + T.get(3,2)*T.get(4,3)*T.get(2,4) + T.get(4,2)*T.get(2,3) *T.get(3,4) -T.get(4,2)*T.get(3,3)*T.get(2,4) - T.get(3,2)*T.get(2,3)*T.get(4,4) - T.get(2,2)*T.get(4,3)*T.get(3,4))/det);
    u.set_Y(-(T.get(2,1)*T.get(3,3)*T.get(4,4) + T.get(3,1)*T.get(4,3)*T.get(2,4) + T.get(4,1)*T.get(2,3) *T.get(3,4) -T.get(4,1)*T.get(3,3)*T.get(2,4) - T.get(3,1)*T.get(2,3)*T.get(4,4) - T.get(2,1)*T.get(4,3)*T.get(3,4))/det);
    u.set_Z((T.get(2,1)*T.get(3,2)*T.get(4,4) + T.get(3,1)*T.get(4,2)*T.get(2,4) + T.get(4,1)*T.get(2,2) *T.get(3,4) -T.get(4,1)*T.get(3,2)*T.get(2,4) - T.get(3,1)*T.get(2,2)*T.get(4,4) - T.get(2,1)*T.get(4,2)*T.get(3,4))/det);
    u.set_D(-(T.get(2,1)*T.get(3,2)*T.get(4,3) + T.get(3,1)*T.get(4,2)*T.get(2,3) + T.get(4,1)*T.get(2,2) *T.get(3,3) -T.get(4,1)*T.get(3,2)*T.get(2,3) - T.get(3,1)*T.get(2,2)*T.get(4,3) - T.get(2,1)*T.get(4,2)*T.get(3,3))/det);
    nueva.setVector1(u);
    
    u.set_X(-(T.get(1,2)*T.get(3,3)*T.get(4,4) + T.get(4,2)*T.get(1,3)*T.get(3,4) + T.get(3,2)*T.get(4,3) *T.get(1,4) -T.get(4,2)*T.get(3,3)*T.get(1,4) - T.get(3,2)*T.get(1,3)*T.get(4,4) - T.get(1,2)*T.get(4,3)*T.get(3,4))/det);
    u.set_Y((T.get(1,1)*T.get(3,3)*T.get(4,4) + T.get(3,1)*T.get(4,3)*T.get(1,4) + T.get(4,1)*T.get(1,3) *T.get(3,4) -T.get(4,1)*T.get(3,3)*T.get(1,4) - T.get(3,1)*T.get(1,3)*T.get(4,4) - T.get(1,1)*T.get(4,3)*T.get(3,4))/det);    
    u.set_Z(-(T.get(1,1)*T.get(3,2)*T.get(4,4) + T.get(3,1)*T.get(4,2)*T.get(1,4) + T.get(4,1)*T.get(1,2) *T.get(3,4) -T.get(4,1)*T.get(3,2)*T.get(1,4) - T.get(3,1)*T.get(1,2)*T.get(4,4) - T.get(1,1)*T.get(4,2)*T.get(3,4))/det);    
    u.set_D((T.get(1,1)*T.get(3,2)*T.get(4,3) + T.get(3,1)*T.get(4,2)*T.get(1,3) + T.get(4,1)*T.get(1,2) *T.get(3,3) -T.get(4,1)*T.get(3,2)*T.get(1,3) - T.get(3,1)*T.get(1,2)*T.get(4,3) - T.get(1,1)*T.get(4,2)*T.get(3,3))/det);
    nueva.setVector2(u);
    
    u.set_X((T.get(1,2)*T.get(2,3)*T.get(4,4) + T.get(2,2)*T.get(4,3)*T.get(1,4) + T.get(4,2)*T.get(1,3) *T.get(2,4) -T.get(2,2)*T.get(1,3)*T.get(4,4) - T.get(1,2)*T.get(4,3)*T.get(2,4) - T.get(4,2)*T.get(2,3)*T.get(1,4))/det);    
    u.set_Y(-(T.get(1,1)*T.get(2,3)*T.get(4,4) + T.get(2,1)*T.get(4,3)*T.get(1,4) + T.get(4,1)*T.get(1,3) *T.get(2,4) -T.get(4,1)*T.get(2,3)*T.get(1,4) - T.get(2,1)*T.get(1,3)*T.get(4,4) - T.get(1,1)*T.get(4,3)*T.get(2,4))/det);    
    u.set_Z((T.get(1,1)*T.get(2,2)*T.get(4,4) + T.get(2,1)*T.get(4,2)*T.get(1,4) + T.get(4,1)*T.get(1,2) *T.get(2,4) -T.get(4,1)*T.get(2,2)*T.get(1,4) - T.get(2,1)*T.get(1,2)*T.get(4,4) - T.get(1,1)*T.get(4,2)*T.get(2,4))/det);   
    u.set_D(-(T.get(1,1)*T.get(2,2)*T.get(4,3) + T.get(2,1)*T.get(4,2)*T.get(1,3) + T.get(4,1)*T.get(1,2) *T.get(2,3) -T.get(4,1)*T.get(2,2)*T.get(1,3) - T.get(2,1)*T.get(1,2)*T.get(4,3) - T.get(1,1)*T.get(4,2)*T.get(2,3))/det);
    nueva.setVector3(u);
    
    u.set_X(-(T.get(1,2)*T.get(2,3)*T.get(3,4) + T.get(2,2)*T.get(3,3)*T.get(1,4) + T.get(3,2)*T.get(1,3) *T.get(2,4) -T.get(3,2)*T.get(2,3)*T.get(1,4) - T.get(2,2)*T.get(1,3)*T.get(3,4) - T.get(1,2)*T.get(3,3)*T.get(2,4))/det);    
    u.set_Y((T.get(1,1)*T.get(2,3)*T.get(3,4) + T.get(2,1)*T.get(3,3)*T.get(1,4) + T.get(3,1)*T.get(1,3) *T.get(2,4) -T.get(3,1)*T.get(2,3)*T.get(1,4) - T.get(2,1)*T.get(1,3)*T.get(3,4) - T.get(1,1)*T.get(3,3)*T.get(2,4))/det);    
    u.set_Z(-(T.get(1,1)*T.get(2,2)*T.get(3,4) + T.get(2,1)*T.get(3,2)*T.get(1,4) + T.get(3,1)*T.get(1,2) *T.get(2,4) -T.get(3,1)*T.get(2,2)*T.get(1,4) - T.get(2,1)*T.get(1,2)*T.get(3,4) - T.get(1,1)*T.get(3,2)*T.get(2,4))/det);  
    u.set_D((T.get(1,1)*T.get(2,2)*T.get(3,3) + T.get(2,1)*T.get(3,2)*T.get(1,3) + T.get(3,1)*T.get(1,2) *T.get(2,3) -T.get(3,1)*T.get(2,2)*T.get(1,3) - T.get(2,1)*T.get(1,2)*T.get(3,3) - T.get(1,1)*T.get(3,2)*T.get(2,3))/det);
    nueva.setVector4(u);
    
    Vector tt1 = nueva.getVector(1);
    Vector tt2 = nueva.getVector(2);
    Vector tt3 = nueva.getVector(3);
    Vector tt4 = nueva.getVector(4);
    
    Vector n;
    n.set_values(tt1.get(1),tt2.get(1),tt3.get(1),tt4.get(1));
    nueva.setVector1(n);
    
    n.set_values(tt1.get(2),tt2.get(2),tt3.get(2),tt4.get(2));
    nueva.setVector2(n);
    
    n.set_values(tt1.get(3),tt2.get(3),tt3.get(3),tt4.get(3));
    nueva.setVector3(n);
    
    n.set_values(tt1.get(4),tt2.get(4),tt3.get(4),tt4.get(4));
    nueva.setVector4(n);
    
   
    return nueva;    
}

Vector Montecarlo::multiplicarMatrizValores(Matriz T1,double x1, double x2,double x3){
    Vector w;
    
    w.set_X(T1.get(1,1)*x1+T1.get(1,2)*x2+T1.get(1,3)*x3);
    w.set_Y(T1.get(2,1)*x1+T1.get(2,2)*x2+T1.get(2,3)*x3);
    w.set_Z(T1.get(3,1)*x1+T1.get(3,2)*x2+T1.get(3,3)*x3);
    w.set_D(T1.get(4,1)*x1+T1.get(4,2)*x2+T1.get(4,3)*x3);
    
    return w;
}
list<Vector> Montecarlo::calcularw(){
    list<Matriz> matT= calcularT();
    list<Vector> vect;
    for(int i=0;i<matT.size();i++){
        Matriz T1 = inversaT(matT.back());
        matT.pop_back();
        Vector wi = multiplicarMatrizValores(T1,sin(getInclination())*cos(getAzimuth()),sin(getInclination())*sin(getAzimuth()),cos(getInclination()));
        vect.push_back(wi);
    }
   return vect;
}

