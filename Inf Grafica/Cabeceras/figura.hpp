/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet 681721
 *
 * Clase que representa a una figura geometrica. Cada gifura posee un coeficiente de refraccion del medio,
 * y un coefeciente para medir lo que refleja y lo que refracta.
 */
#ifndef FIGURA_H
#define FIGURA_H

#include "color.hpp"
#include "rayo.hpp"
#include "luz.hpp"

class Figura {

	
protected:

	Color color;
	bool luz = false;

	//0 -> Phong, 1 -> Ward
	int brdf = 0;
	int idFigura;
	double reflejo, refraccion, kr = 1; //Kr es coeficiente de refraccion del medio de la figura. Valor por defecto = Valor en el aire.

public:

	/*
	 * Getters.
	 */
	Color getColor();
	double getReflejo();
	double getRefraccion();
	double getCoefRefraccion();
	int figuraId();
	int getBRDF();

	/*
	 * Setters.
	 */
	void setReflejo(double);
	void setRefraccion(double);
	void setCoefRefraccion(double);
	void setId(int);
	void setColor(Color);
	void setBRDF(int);

	bool isBox();

	bool isLuz();

	virtual void setLuz(Luz) = 0;

	/*
	 * 
	 */
	/*
	 * Devuelve la distancia a la que intersecta el rayo r la figura, o devuelve un valor negativo si no lo hace.
	 */
	virtual double intersectar(Rayo r) = 0;

	/*
	 * Devuelve la normal de la figura en el punto dado.
	 */
	virtual Vector normal(Punto) = 0;
};

#endif