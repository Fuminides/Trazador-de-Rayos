#include "esfera.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include "punto.hpp"
    /*
     * Devuelve el radio de la esfera
     */ 
    float Esfera::getRadio(){
        return radio;
    }
    /*
     * Asigna un valor al radio 
     */
    void Esfera::setRadio(float r){
        radio=r;
    }
    /*
     * Devuelve el origen de la esfera en el plano
     */
    Punto Esfera::getOrigen(){
        return origen;
    }
    /*
     * Asigna el valor del origen de la esfera
     */
    void Esfera::setOrigen(float x, float y, float z){
            origen.set_values(x,y,z);
    }
    /*
     * Devuelve la superficie de la esfera
     */
    float Esfera::superficie(){
        return radio * radio * M_PI;
    }

    void Esfera::setColor(Color c){
        color = c;
    }

    Color Esfera::getColor(){
        return color;
    }