#ifndef esfera_h_
#define esfera_h_

#include <math.h>
#include "punto.hpp"
#include "vector.hpp"
#include "color.hpp"
#include "figura.hpp"
#include "operadorVector.hpp"

 class Esfera : public Figura{

        float radio;
        Punto origen;
        Color color;

        float puntoPorPunto(Punto p1, Punto p2);
        float puntoPorVector(Punto p1, Vector p2);

     public:
        
        /*
         * Devuelve la distancia a la que un Rayo r intercepta a una esfera f, -1 en caso de que no 
         * la intercepte
         */
        float intersectar(Rayo);
        /*
         * Devuelve el radio de la esfera
         */ 
        float getRadio();
        /*
         * Asigna un valor al radio 
         */
        void setRadio(float r);
        /*
         * Devuelve la superficie de la esfera
         */
        float superficie();
        /*
         * Devuelve el origen de la esfera en el plano
         */
        Punto getOrigen();
        /*
         * Asigna el valor del origen de la esfera
         */
        void setOrigen(float x, float y,float z);
        /*
         * Devuelve la normal de la figura dado un punto
         */
         Vector normal(Punto);

 };

 #endif