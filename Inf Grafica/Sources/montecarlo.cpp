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
Matriz montecarlo::calcularT(){
    Vector azar;
    azar.set_values(0,1,0);
    Vector u = productoVectorial(azar,n);
    Vector v = productoVectorial(n,u);
    cout<<"El vector u es:"<<u.getX()<<","<<u.getY()<<","<<u.getZ()<<","<<u.getD()<<endl;
    cout<<"El vector v es:"<<v.getX()<<","<<v.getY()<<","<<v.getZ()<<","<<v.getD()<<endl;
    Matriz T;
    T.set_valuesColum(u,v,n,x);
    Vector u1 =T.getVector1();
    Vector u2 =T.getVector2();
    Vector u3 =T.getVector3();
    Vector u4 =T.getVector4();
    cout<<"El vector u1 es:"<<u1.getX()<<","<<u1.getY()<<","<<u1.getZ()<<","<<u1.getD()<<endl;
    cout<<"El vector u2 es:"<<u2.getX()<<","<<u2.getY()<<","<<u2.getZ()<<","<<u2.getD()<<endl;
    cout<<"El vector u3 es:"<<u3.getX()<<","<<u3.getY()<<","<<u3.getZ()<<","<<u3.getD()<<endl;
    cout<<"El vector u4 es:"<<u4.getX()<<","<<u4.getY()<<","<<u4.getZ()<<","<<u4.getD()<<endl;
    return T;
}

Matriz montecarlo::inversaT(Matriz T){
    double det = determinante(T,4);
    Matriz nueva;
    Vector u;
    
    u.set_X((T.getVector2().getY()*T.getVector3().getZ()-T.getVector2().getZ()*T.getVector3().getY())/det);
    u.set_Y(-(T.getVector1().getY()*T.getVector3().getZ()-T.getVector3().getY()*T.getVector1().getZ())/det);
    u.set_Z((T.getVector1().getY()*T.getVector2().getZ()-T.getVector2().getY()*T.getVector1().getZ())/det);
    u.set_D(-(T.getVector1().getY()*T.getVector2().getZ()*T.getVector3().getD()+T.getVector1().getZ()*T.getVector2().getD()*T.getVector3().getY()+T.getVector2().getY()*T.getVector3().getZ()*T.getVector1().getD()-T.getVector3().getY()*T.getVector2().getZ()*T.getVector1().getD()-T.getVector3().getZ()*T.getVector2().getD()*T.getVector1().getY()-T.getVector2().getY()*T.getVector1().getZ()*T.getVector3().getD())/det);
    nueva.setVector1(u);
    
    u.set_X(-(T.getVector2().getX()*T.getVector3().getZ()-T.getVector3().getX()*T.getVector2().getZ())/det);
    u.set_Y((T.getVector1().getX()*T.getVector3().getZ()-T.getVector3().getX()*T.getVector1().getZ())/det);
    u.set_Z(-(T.getVector1().getX()*T.getVector2().getZ()-T.getVector2().getX()*T.getVector1().getZ())/det);
    u.set_D((T.getVector1().getX()*T.getVector2().getZ()*T.getVector3().getD()+T.getVector1().getZ()*T.getVector2().getD()*T.getVector3().getX()+T.getVector2().getX()*T.getVector3().getZ()*T.getVector1().getX()-T.getVector1().getD()*T.getVector2().getZ()*T.getVector3().getX()-T.getVector3().getZ()*T.getVector2().getD()*T.getVector1().getX()-T.getVector2().getX()*T.getVector1().getZ()*T.getVector3().getD())/det);
    nueva.setVector2(u);
    
    u.set_X((T.getVector2().getX()*T.getVector3().getY()-T.getVector3().getX()*T.getVector2().getY())/det);
    u.set_Y(-(T.getVector1().getX()*T.getVector3().getY()-T.getVector3().getX()*T.getVector1().getY())/det);
    u.set_Z((T.getVector1().getX()*T.getVector2().getY()-T.getVector2().getX()*T.getVector1().getY())/det);
    u.set_D(-(T.getVector1().getX()*T.getVector2().getY()*T.getVector3().getD()+T.getVector2().getX()*T.getVector3().getY()*T.getVector1().getD()+T.getVector3().getX()*T.getVector1().getY()*T.getVector2().getD()-T.getVector3().getX()*T.getVector2().getY()*T.getVector1().getD()-T.getVector2().getX()*T.getVector1().getY()*T.getVector3().getD()-T.getVector3().getY()*T.getVector2().getD()*T.getVector1().getX())/det);
    nueva.setVector3(u);
    
    u.set_X(0.0);
    u.set_Y(0.0);
    u.set_Z(0.0);
    u.set_D((T.getVector1().getX()*T.getVector2().getY()*T.getVector3().getD() + T.getVector1().getY()*T.getVector2().getZ()*T.getVector3().getX() + T.getVector2().getX()*T.getVector3().getY()*T.getVector1().getZ() - T.getVector3().getX()*T.getVector2().getY()*T.getVector1().getZ() - T.getVector3().getY()*T.getVector2().getZ()*T.getVector1().getX() - T.getVector2().getX()*T.getVector1().getY()*T.getVector3().getD())/det);
    nueva.setVector4(u);
   
    return nueva;    
}

double montecarlo::determinante(Matriz aux,int orden){
    double det = 0.0;
    for(int j=0;j<orden;j++){
        if(j==0){
            det =  det + aux.getVector1().getX() + cofactor(aux,orden,0,j);
        }
        else if(j==1){
            det =  det + aux.getVector1().getY() + cofactor(aux,orden,0,j);
        }
        else if(j==2){
            det =  det + aux.getVector1().getZ() + cofactor(aux,orden,0,j);
        }
        else{
            det =  det + aux.getVector1().getD() + cofactor(aux,orden,0,j);
        }
    }
    return det;
}

double montecarlo ::cofactor(Matriz aux,int orden,int fila,int columna){
    Matriz subm;
    Vector u;
    int n=orden-1;
    int x=0;
    int y=0;
    for(int i=0;i<orden;i++){
        for(int j=0;j<orden;j++){
            if(i!=fila && j!=columna){
                if(y==0){
                    if(j==0){
                        u.set_X(aux.getVector(i).getX());
                    }
                    else if(j==1){
                        u.set_X(aux.getVector(i).getY());
                    }
                    else if(j==2){
                        u.set_X(aux.getVector(i).getZ());
                    }
                    else {
                        u.set_X(aux.getVector(i).getD());
                    }
                }
                else if(y==1){
                    if(j==0){
                        u.set_Y(aux.getVector(i).getX());
                    }
                    else if(j==1){
                        u.set_Y(aux.getVector(i).getY());
                    }
                    else if(j==2){
                        u.set_Y(aux.getVector(i).getZ());
                    }
                    else {
                        u.set_Y(aux.getVector(i).getD());
                    }
                }
                else if(y==2){
                    if(j==0){
                        u.set_Z(aux.getVector(i).getX());
                    }
                    else if(j==1){
                        u.set_Z(aux.getVector(i).getY());
                    }
                    else if(j==2){
                        u.set_Z(aux.getVector(i).getZ());
                    }
                    else {
                        u.set_Z(aux.getVector(i).getD());
                    }
                }
                else {
                    if(j==0){
                        u.set_D(aux.getVector(i).getX());
                    }
                    else if(j==1){
                        u.set_D(aux.getVector(i).getY());
                    }
                    else if(j==2){
                        u.set_D(aux.getVector(i).getZ());
                    }
                    else {
                        u.set_D(aux.getVector(i).getD());
                    }
                }
                subm.setVector(x+1,u);
                y++;
                if(y>=n){
                    x++;
                    y=0;
                }
                
            }
        }
    }
    return pow(-1.0,fila+columna) * determinante(subm,n);
}
Matriz montecarlo::multiplicarMatrizValores(Matriz T1,double x1, double x2,double x3){
    Matriz final;
    for(int i=1;i<=4;i++){
       for(int j=1;j<=4;j++){
            //Falta por hacer esto.
        } 
    }
    return final;
}
Matriz montecarlo::calcularw(){
    Matriz T = calcularT();
    Matriz T1 = inversaT(T);
    //como lo multiplico si los tamaños no coinciden
    Matriz wi = multiplicarMatrizValores(T1,sin(getInclination())*cos(getAzimuth()),sin(getInclination())*sin(getAzimuth()),cos(getInclination()));
}

