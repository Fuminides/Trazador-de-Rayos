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

void Color::multiplicar(float k){
	red = (unsigned char) (int) red * k;
	green = (unsigned char) (int) green * k;
	blue = (unsigned char) (int) blue * k;

}

void Color::sumar(Color c){
	red = c.red;
	green = c.green;
	blue = c.blue;
}
