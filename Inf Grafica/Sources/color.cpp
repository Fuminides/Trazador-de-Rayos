#include "color.hpp"

void Color::set_values(int rojo, int verde, int azul){
	red = rojo;
	green = verde;
	blue = azul;
}

unsigned char Color::splashR(){
	return  red;
}
unsigned char Color::splashG(){
	return  green;
}
unsigned char Color::splashB(){
	return  blue;
}

void Color::multiplicar(double k){
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

void Color::sumar(Color c){
	if ( red + c.red > 255) { red = 255; }
	else {red += c.red;}
	if ( green +c.green > 255) { green = 255; } 
	else { green += c.green; }
	if ( blue + c.blue > 255) { blue = 255; }
	else{ blue += c.blue; }
}

std::string Color::to_string(){
	return "R: " + std::to_string(red) + ", G: " + std::to_string(green) + ", B: " + std::to_string(blue);
}
