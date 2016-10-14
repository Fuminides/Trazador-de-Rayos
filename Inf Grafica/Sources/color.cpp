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

/*
std::string Color::splash(){
	return  std::to_string(red) + " " +  std::to_string(green) + " " +  std::to_string(blue) + " ";  
}*/