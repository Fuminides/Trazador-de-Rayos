#include "luz.hpp"

Color Luz::getColor(){
	return color;
}

void Luz::atenuar(double distancia){
	double ajuste = (potencia * potencia) / (distancia * distancia);
	color.multiplicar(ajuste);
}

Punto Luz::getOrigen(){
	return origen;
}

void Luz::set_values(Punto p, Color c, int pot){
	origen = p;
	color = c;
	potencia = pot;
}