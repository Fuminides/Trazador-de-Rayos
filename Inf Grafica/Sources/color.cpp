#include "color.hpp"

void Color::set_values(int rojo, int verde, int azul){
	red = rojo;
	green = verde;
	blue = azul;
}

std::string Color::splash(){
	return  std::to_string(red) + " " +  std::to_string(green) + " " +  std::to_string(blue) + " ";  
}