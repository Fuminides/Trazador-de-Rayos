/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet 681721
 *
 * Clase que representa a cualquier elemento con x, y, z. Es decir, que
 * este definido por tres coordenadas cartesianas.
 */
#include "objeto_cartesiano.hpp"

float ObjetoCartesiano::getX() {return valores[0];}
float ObjetoCartesiano::getY() {return valores[1];}
float ObjetoCartesiano::getZ() {return valores[2];}
float ObjetoCartesiano::getD() {return valores[3];}



void ObjetoCartesiano::set_values(float _x, float _y, float _z){
	valores[0] = _x;
	valores[1] = _y;
	valores[2] = _z;
	if ( esVector() ){
		valores[3] = 0;
	}
	else{
		valores[3] = 1;
	}

}

void ObjetoCartesiano::set_values(float _x, float _y, float _z, float _d){
	valores[0] = _x;
	valores[1] = _y;
	valores[2] = _z;
	valores[3] = _d;
}

void ObjetoCartesiano::set_X(float valor){
	valores[0] = valor;
}

void ObjetoCartesiano::set_Y(float valor){
	valores[1] = valor;
}

void ObjetoCartesiano::set_Z(float valor){
	valores[2] = valor;
}

float ObjetoCartesiano::get(int indice){
	return valores[indice-1];
}