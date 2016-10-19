#ifndef LUZ_H
#define LUZ_H

#include "color.hpp"
#include "rayo.hpp"

class Luz  {
	Color color;
	Punto origen;

public:
	void set_values(Punto, Color);

	Color getColor();
	Punto getOrigen();
};

#endif