/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet 681721
 *
 * Clase que representa a cualquier elemento con x, y, z. Es decir, que
 * este definido por tres coordenadas cartesianas.
 */
#include "objeto_cartesiano.h"

float ObjetoCartesiano::getX() {return x;}
float ObjetoCartesiano::getY() {return y;}
float ObjetoCartesiano::getZ() {return z;}


void ObjetoCartesiano::set_values(float _x, float _y, float _z){
	x = _x;
	y = _y;
	z = _z;
}

void ObjetoCartesiano::set_X(float valor){
	x = valor;
}

void ObjetoCartesiano::set_Y(float valor){
	y = valor;
}

void ObjetoCartesiano::set_Z(float valor){
	z = valor;
}
