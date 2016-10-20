#ifndef COLOR_H_
#define COLOR_H_ 

#include <string>
#include <iostream>

class Color {
	unsigned char red, green, blue;

	public:
		void set_values(int,int,int);
		unsigned char splashR();
		unsigned char splashG();
		unsigned char splashB();

		void multiplicar(float k);
		void sumar(Color); 

		std::string to_string();

		void atenuar(float);

};

#endif