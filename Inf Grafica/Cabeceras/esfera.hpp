#ifndef esfera_h_
#define esfera_h_

#include "punto.hpp"
#include "color.hpp"

 class Esfera {
     private:    
         float radio;
         Punto origen;
         Color color;
     public:
        void setColor(Color);
        float getRadio();
        void setRadio(float r);
        float superficie();
        Punto getOrigen();
        void setOrigen(float x, float y,float z);

        Color getColor();
 };

 #endif