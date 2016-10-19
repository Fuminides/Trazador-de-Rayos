#ifndef FIGURA_H
#define FIGURA_H

#include "color.hpp"
#include "rayo.hpp"

class Figura {

	Color color;

protected:

	int idFigura;

public:

	int figuraId();
	void setColor(Color);
	Color getColor();

	virtual float intersectar(Rayo r) = 0;
	virtual Vector normal(Punto) = 0;
};

#endif