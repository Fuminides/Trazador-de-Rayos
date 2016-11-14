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
using namespace std;


montecarlo::montecarlo(double i,double a,Vector px, Vector normal)
{
    inclination=i;
    azimuth = a;
    n = normal;
    x = px;
}
double montecarlo::getInclination(){
    return inclination;
}
double montecarlo::getAzimuth(){
    return azimuth;
}
Vector montecarlo::getn(){
    return n;
}
Vector montecarlo::getx(){
    return x;
}
Matriz montecarlo::calcularT(){
    Vector azar;
    azar.set_values(0,1,0);
    Vector n= getn();
    Vector x = getx();
    
    Vector u = productoVectorial(azar,n);
    Vector v = productoVectorial(n,u);
    
    Matriz T;
    T.set_valuesColum(u,v,n,x);
       
    return T;
}

Matriz montecarlo::inversaT(Matriz T){
    double det = T.get(1,1)*T.get(2,2)*T.get(3,3)-T.get(3,1)*T.get(2,2)*T.get(1,3);
    Matriz nueva;
    Vector u;
    
    u.set_X((T.get(2,2)*T.get(3,3)-T.get(2,3)*T.get(3,2))/det);
    u.set_Y(-(T.get(1,2)*T.get(3,3)-T.get(3,2)*T.get(1,3))/det);
    u.set_Z((T.get(1,2)*T.get(2,3)-T.get(2,2)*T.get(1,3))/det);
    u.set_D(-(T.get(1,2)*T.get(2,3)*T.get(3,4)+T.get(1,3)*T.get(2,3)*T.get(3,2)+T.get(2,2)*T.get(3,3)*T.get(1,4)-T.get(3,2)*T.get(2,3)*T.get(1,4)-T.get(3,3)*T.get(2,4)*T.get(1,2)-T.get(2,2)*T.get(1,3)*T.get(3,4))/det);
    nueva.setVector1(u);
    
    u.set_X(-(T.get(2,1)*T.get(3,3)-T.get(3,1)*T.get(2,3))/det);
    u.set_Y((T.get(1,1)*T.get(3,3)-T.get(3,1)*T.get(1,3))/det);
    u.set_Z(-(T.get(1,1)*T.get(2,3)-T.get(2,1)*T.get(1,3))/det);
    u.set_D((T.get(1,1)*T.get(2,3)*T.get(3,4)+T.get(1,3)*T.get(2,4)*T.get(3,1)+T.get(2,1)*T.get(3,3)*T.get(1,1)-T.get(1,4)*T.get(2,3)*T.get(3,1)-T.get(3,3)*T.get(2,4)*T.get(1,1)-T.get(2,1)*T.get(1,3)*T.get(3,4))/det);
    nueva.setVector2(u);
    
    u.set_X((T.get(2,1)*T.get(3,2)-T.get(3,1)*T.get(2,2))/det);
    u.set_Y(-(T.get(1,1)*T.get(3,2)-T.get(3,1)*T.get(1,2))/det);
    u.set_Z((T.get(1,1)*T.get(2,2)-T.get(2,1)*T.get(1,2))/det);
    u.set_D(-(T.get(1,1)*T.get(2,2)*T.get(3,4)+T.get(2,1)*T.get(3,2)*T.get(1,4)+T.get(3,1)*T.get(1,2)*T.get(2,4)-T.get(3,1)*T.get(2,2)*T.get(1,4)-T.get(2,1)*T.get(1,2)*T.get(3,4)-T.get(3,2)*T.get(2,4)*T.get(1,1))/det);
    nueva.setVector3(u);
    
    u.set_X(0.0);
    u.set_Y(0.0);
    u.set_Z(0.0);
    u.set_D((T.get(1,1)*T.get(2,2)*T.get(3,3) + T.get(1,2)*T.get(2,3)*T.get(3,1) + T.get(2,1)*T.get(3,2)*T.get(1,3) - T.get(3,1)*T.get(2,2)*T.get(1,3) - T.get(3,2)*T.get(2,3)*T.get(1,1) - T.get(2,1)*T.get(1,2)*T.get(3,3))/det);
    nueva.setVector4(u);
   
    return nueva;    
}

Vector montecarlo::multiplicarMatrizValores(Matriz T1,double x1, double x2,double x3){
    //como lo multiplico si los tamaños no coinciden?? El valor T1.get(1,4) lo multiplico x 0, o x que?
    Vector w;
    
    w.set_X(T1.get(1,1)*x1+T1.get(1,2)*x2+T1.get(1,3)*x3);
    w.set_Y(T1.get(2,1)*x1+T1.get(2,2)*x2+T1.get(2,3)*x3);
    w.set_Z(T1.get(3,1)*x1+T1.get(3,2)*x2+T1.get(3,3)*x3);
    w.set_D(T1.get(4,1)*x1+T1.get(4,2)*x2+T1.get(4,3)*x3);
    
    return w;
}
Vector montecarlo::calcularw(){
    Matriz T = calcularT();
    Matriz T1 = inversaT(T);
    Vector wi = multiplicarMatrizValores(T1,sin(getInclination())*cos(getAzimuth()),sin(getInclination())*sin(getAzimuth()),cos(getInclination()));
    return wi;
}

