#include "operadorEscena.hpp"
#include "rayo.hpp"
#include "esfera.hpp"
#include <vector>
#include <stdio.h>
#include <math.h>
/*
 * Devuelve la distancia a la que un Rayo r intercepta a una esfera f, -1 en caso de que no 
 * la intercepte
 */
float operadorEscena::interseccionRayoEsfera(Rayo r,Esfera a){
    //Estos calculos no estan bien , pero es por tener la idea hecha.
    float O=r.getOrigen().getX()+r.getOrigen().getY()+r.getOrigen().getZ();
    float C=a.getOrigen().getX()+a.getOrigen().getY()+a.getOrigen().getZ();
    float OC=r.getOrigen().getX()*a.getOrigen().getX()+r.getOrigen().getY()*a.getOrigen().getY()+r.getOrigen().getZ()*a.getOrigen().getZ();
    float OO=r.getOrigen().getX()*r.getOrigen().getX()+r.getOrigen().getY()*r.getOrigen().getY()+r.getOrigen().getZ()*r.getOrigen().getZ();
    float CC=a.getOrigen().getX()*a.getOrigen().getX()+a.getOrigen().getY()*a.getOrigen().getY()+a.getOrigen().getZ()*a.getOrigen().getZ();
    float raiz=(2*O-2*C)*(2*O-2*C)-4*OO*(OO+CC-2*OC-(a.getRadio()*a.getRadio()));
    if (raiz<0){
        return -1;
    }
    else{
        raiz=sqrt(raiz);
        float p1=-(2*O-2*C);
        float d1=(p1+raiz)/(2*OO);
        float d2=(p1-raiz)/(2*OO);;
        if (d1 >0 && d1<=d2){
           return d1;
        }
        else if(d2>0 && d2<d1){
             return d2; 
        }
        else{
            return -1;
        }
    }
    
}
/*
 * Devuelve la distancia de la esfera mas cercana con la que intercepta un rayo r
 */
float operadorEscena::intersecciones(Rayo r){
    float menor=0;
    Esfera eMenor;
    for (int i=0; i< esferas.size();i++){
        Esfera a=esferas[i];
        float d= interseccionRayoEsfera(r,a);
        if (d>0 && d< menor){
            eMenor=a;
            menor=d;
        }
    }
    return menor;
    
}
/*
 * Añade un rayo al array de rayos de la escena
 */
void operadorEscena::anyadirRayo(Rayo r){
    rayos.push_back(r);
    rayos.resize(rayos.size()+1);
}
/*
 * Añade una esfera al array de esferas de la escena
 */
void operadorEscena::anyadirEsfera(Esfera f){
    esferas.push_back(f);
    esferas.resize(esferas.size()+1);
}
/*
 * Devuelve el numero de rayos que hay en la escena
 */
int operadorEscena::tamRayos(){
    return rayos.size();
    
}
/*
 * Devuelve el numero de esferas que hay en la escena
 */
int operadorEscena::tamEsferas(){
    return esferas.size();
}