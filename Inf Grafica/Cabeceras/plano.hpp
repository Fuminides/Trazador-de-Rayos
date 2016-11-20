/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet 681721
 *
 * Clase que representa a un plano.
 */
#ifndef PLANO_H_
#define PLANO_H_ 

#include "figura.hpp"
#include "vector.hpp"
#include "punto.hpp"
#include "rayo.hpp"
#include "operadorVector.hpp"

#include <math.h>

class Plano : public Figura
{

	Vector vNormal, vectorX, vectorY;
	Luz luces[5];
	Punto origen;
	double altura, anchura, d;

public:

	void set_values(Punto, Vector, Vector, double, Vector, double);

	double intersectar(Rayo);
	Vector normal(Punto);

	void setLuz(Luz);
};

#endif