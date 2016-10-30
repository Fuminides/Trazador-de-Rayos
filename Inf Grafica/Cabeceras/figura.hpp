#ifndef FIGURA_H
#define FIGURA_H

#include "color.hpp"
#include "rayo.hpp"

class Figura {

	Color color;

protected:

	int idFigura;
	double reflejo, refraccion, kr = 1;

public:

	int figuraId();
	void setId(int);
	void setColor(Color);
	Color getColor();

	double getReflejo();
	double getRefraccion();
	double getCoefRefraccion();

	void setReflejo(double);
	void setRefraccion(double);
	void setCoefRefraccion(double);

	virtual double intersectar(Rayo r) = 0;
	virtual Vector normal(Punto) = 0;
};

#endif