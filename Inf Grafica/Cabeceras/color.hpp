/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet 681721
 *
 * Clase que representa a una camara, con un origen, tres vectores, un tamano de imagen
 * y una resolucion.
 */
#ifndef COLOR_H_
#define COLOR_H_ 

#include <string>
#include <iostream>

class Color {
	unsigned char red, green, blue;
	

	public:
		void set_values(int,int,int);

		/*
		 * Getters. (En formato de un bit)
		 */
		unsigned char splashR();
		unsigned char splashG();
		unsigned char splashB();

		/**
		 * Operaciones con los RGB. En caso de overflow dejan el valor maximo (255).
		 */
		void multiplicar(double k);
		void sumar(Color); 

		std::string to_string();


};

#endif