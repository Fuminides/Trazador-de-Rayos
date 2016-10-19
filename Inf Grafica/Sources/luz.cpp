#include "luz.hpp"

Color Luz::getColor(){
	return color;
}

Punto Luz::getOrigen(){
	return origen;
}

void Luz::set_values(Punto p, Color c){
	origen = p;
	color = c;
}