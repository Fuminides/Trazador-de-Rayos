#ifndef camara_h_
#define camara_h_

#include "vector.hpp"
#include "punto.hpp"

class Camara {

	Punto posicion;
	Vector ejes[3];

	public:

		void set_values(Punto, Vector, Vector, Vector);

		Vector getVX();
		Vector getVY();
		Vector getVZ();
		Punto getPosicion();

		void setVX(Vector);
		void setVY(Vector);
		void setVZ(Vector);
		void setPosicion(Punto);

		
};

#endif