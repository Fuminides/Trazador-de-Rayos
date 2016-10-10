#include "rayo.hpp"
#include "punto.hpp"


void Rayo::setOrigen(Punto o){
        origen=o;
}
void Rayo::setDistancia(float d){
        distancia=d;    
}
Punto Rayo::getOrigen(){
        return origen;
}
float Rayo::getDistancia(){
        return distancia;
}

