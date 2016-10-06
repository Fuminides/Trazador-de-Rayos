#include "camara.hpp"

void Camara::set_values(Punto p, Vector v1, Vector v2, Vector v3){
	posicion = p;
	ejes[0] = v1;
	ejes[1] = v2;
	ejes[2] = v3;
}

Vector Camara::getVX() { return ejes[0]; }
Vector Camara::getVY() { return ejes[1]; }
Vector Camara::getVZ() { return ejes[2]; }

void Camara::setVX(Vector v) { ejes[0] = v; }
void Camara::setVY(Vector v) { ejes[1] = v; }
void Camara::setVZ(Vector v) { ejes[2] = v; }

Punto Camara::getPosicion() { return posicion; }
void Camara::setPosicion(Punto p) { posicion = p; }

