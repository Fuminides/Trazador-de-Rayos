#ifndef RAYO_H
#define RAYO_H

#include "punto.hpp"
#include "vector.hpp"

class Rayo{

        Punto origen;
        Vector v;

    public:
    	void set_values(Punto, Vector);

        void setOrigen(Punto);
        void setVector(Vector);

        Punto getOrigen();
        Vector getVector();

};

#endif 
