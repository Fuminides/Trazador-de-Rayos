#include "luz.hpp"

Color Luz::getColor(){
	return color;
}

void Luz::atenuar(double distancia){
	double ajuste = (potencia * potencia) / (distancia * distancia);
	//std::cout << "Pot: "<<std::to_string(potencia)<<" distancia :"<<std::to_string(distancia)<<"Mul: " << std::to_string(ajuste) << "\n";
	color.multiplicar(ajuste);
}

Punto Luz::getOrigen(){
	return origen;
}

void Luz::set_values(Punto p, Color c, double pot){
	origen = p;
	color = c;
	potencia = pot;
}

double Luz::getPotencia(){
	return potencia;
}