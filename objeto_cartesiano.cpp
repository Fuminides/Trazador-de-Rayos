/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet
 *
 * Clase que representa a cualquier elemento con x, y, z. Es decir, que
 * este definido por tres coordenadas cartesianas.
 */
#include "objeto_cartesiano.h"

int ObjetoCartesiano::getX() {return x;}
int ObjetoCartesiano::getY() {return y;}
int ObjetoCartesiano::getZ() {return z;}


void ObjetoCartesiano::set_values(int _x, int _y, int _z){
	x = _x;
	y = _y;
	z = _z;
}

void ObjetoCartesiano::set_X(int valor){
	x = valor;
}

void ObjetoCartesiano::set_Y(int valor){
	y = valor;
}

void ObjetoCartesiano::set_Z(int valor){
	z = valor;
}
