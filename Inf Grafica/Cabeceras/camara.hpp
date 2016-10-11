#ifndef camara_h_
#define camara_h_

#include "vector.hpp"
#include "punto.hpp"
#include "rayo.hpp"
#include "operadorVector.hpp"
#include "operadorEscena.hpp"

#include <iostream>
#include <list>

class Camara {

	int resX, resY;
	float distancia;
	Punto posicion, plano;
	Vector ejes[3];
	

	Rayo trazarRayo(Punto p);
	
	public:

		void set_values(Punto, Vector, Vector, Vector, int, int, float);

		Vector getVX();
		Vector getVY();
		Vector getVZ();
		Punto getPosicion();

		void setVX(Vector);
		void setVY(Vector);
		void setVZ(Vector);
		void setPosicion(Punto);

		std::list<Rayo> trazarRayos();

		
};

#endif