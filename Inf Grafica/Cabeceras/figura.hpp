#ifndef FIGURA_H
#define FIGURA_H

#include "color.hpp"
#include "rayo.hpp"

class Figura {

	Color color;

protected:

	int idFigura;
	double reflejo, refraccion;

public:

	int figuraId();
	void setColor(Color);
	Color getColor();

	double getReflejo();
	double getRefraccion();

	void setReflejo(double);
	void setRefraccion(double);

	virtual double intersectar(Rayo r) = 0;
	virtual Vector normal(Punto) = 0;
};

#endif