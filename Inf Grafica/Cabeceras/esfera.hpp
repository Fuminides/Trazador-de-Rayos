#ifndef esfera_h_
#define esfera_h_
#include "punto.hpp"


 class Esfera {
     private:    
         float radio;
         Punto origen;
     public:
        
        float getRadio();
        void setRadio(float r);
        float superficie();
        Punto getOrigen();
        void setOrigen(float x, float y,float z);
 };

 #endif