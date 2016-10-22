#include "color.hpp"

void Color::set_values(int rojo, int verde, int azul){
	red = rojo;
	green = verde;
	blue = azul;

	redD = rojo * 1.0;
	greenD = verde * 1.0;
	blueD = azul * 1.0;
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
	redD = redD * k;
	if ( redD > 255 ) red = 255, redD = 255.0;
	else{
		red = redD;
		//std::cout << "Red: " << std::to_string(red) << " y con decimales " << std::to_string(redD) << '\n';
	} 
	
	greenD = greenD * k;
	if ( greenD > 255 ) green = 255, greenD = 255.0;
	else{
		green = greenD;
		//std::cout << "Green: " << std::to_string(green) << " y con decimales " << std::to_string(greenD) << '\n';
	} 

	blueD = blueD * k;
	if ( blueD > 255 ) blue = 255, blueD = 255.0;
	else{
		blue = blueD;
		//std::cout << "Blue: " << std::to_string(blue) << " y con decimales " << std::to_string(blueD) << '\n';
	}
}

void Color::sumar(Color c){
	if ( redD + c.red > 255) { //std::cout << "Overflow rojo! "; 
	red = 255; redD = 255.0; }
	else {red += c.red; redD += c.red;}
	if ( green +c.green > 255) { //std::cout << "Overflow verde!"; 
	green = 255; greenD = 255.0; } 
	else { green += c.green; greenD += c.green; }
	if ( blue + c.blue > 255) { //std::cout << "Overflow azul! "; 
	blue = 255; blueD = 255.0;}
	else{ blue += c.blue; blueD += c.blue; }
}

std::string Color::to_string(){
	return "R: " + std::to_string(red) + ", G: " + std::to_string(green) + ", B: " + std::to_string(blue);
}
