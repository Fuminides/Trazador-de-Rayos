#include "color.hpp"

void Color::set_values(int _rojo, int _verde, int _azul, bool n){
	red = _rojo;
	green = _verde;
	blue = _azul;

	rojo = _rojo;
	verde = _verde;
	azul = _azul;

	normaliza = n;
}

unsigned char Color::splashR(){
	if (!normaliza) return  red;
	unsigned char aux = (unsigned char) (((int) rojo) % 255);
	if ( rojo >= 254.5 ) aux = 255;
	return aux;
}
unsigned char Color::splashG(){
	if (!normaliza) return  green;
	unsigned char aux = (unsigned char) (((int) verde) % 255);
	if ( verde >= 254.5 ) aux = 255;
	return aux;
}
unsigned char Color::splashB(){
	if (!normaliza) return  blue;
	unsigned char aux = (unsigned char) (((int) azul) % 255);
	if ( azul >= 254.5 ) aux = 255;
	return aux;
}

void Color::multiplicar(double k){
	if (!normaliza){
		if ( red * k > 255 ) red = 255;
		else{
			red = red * k;
		} 
		
		if ( green * k > 255 ) green = 255;
		else{
			green = green * k;
		} 

		if ( blue * k > 255 ) blue = 255;
		else{
			blue = blue * k;
		}
	}
	else{
		rojo = rojo * k;
		verde = verde * k;
		azul = azul * k;
	}
}

void Color::sumar(Color c){
	if (!normaliza){
		if ( red + c.red > 255) { red = 255; }
		else {red += c.red;}
		if ( green +c.green > 255) { green = 255; } 
		else { green += c.green; }
		if ( blue + c.blue > 255) { blue = 255; }
		else{ blue += c.blue; }
	}
	else{
		rojo += c.rojo;
		verde += c.verde;
		azul += c.azul;
	}
}

std::string Color::to_string(){
	if (!normaliza) return "R: " + std::to_string(red) + ", G: " + std::to_string(green) + ", B: " + std::to_string(blue);
	return "R: " + std::to_string((unsigned char) (((int) rojo) % 255)) + ", G: " + std::to_string((unsigned char) (((int) verde) % 255)) + ", B: " + std::to_string((unsigned char) (((int) azul) % 255))
	+ ", Originales: R: " + std::to_string(rojo) + ", G: " + std::to_string(verde) + ", B: " + std::to_string(azul);
}

void Color::normalizar(double max){
	if ( normaliza ){
		rojo = rojo / max * RANGO;
		azul = azul / max * RANGO;
		verde = verde / max * RANGO;
	}
}

double Color::max(){
	if ( (rojo >= verde) && (rojo >= azul) ) return rojo;
	if ( (azul >= verde) && (azul >= rojo) ) return azul;
	if ( (verde >= rojo) && (verde >= azul) ) return verde;

	std::cout << "No deberia ocurrir\n";
}
