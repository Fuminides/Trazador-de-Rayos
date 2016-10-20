#ifndef LUZ_H
#define LUZ_H

#include "color.hpp"
#include "rayo.hpp"

class Luz  {
	Color color;
	Punto origen;
	double potencia;

public:
	void set_values(Punto, Color, int);

	Color getColor();
	Punto getOrigen();

	void atenuar(double);

};

#endif