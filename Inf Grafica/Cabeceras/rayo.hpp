#ifndef RAYO_H
#define RAYO_H
#include "punto.hpp"
class Rayo{
    private:
        Punto origen;
        float distancia=1;
    public:
        void setOrigen(Punto o);
        void setDistancia(float d);
        Punto getOrigen();
        float getDistancia();

};

#endif 
