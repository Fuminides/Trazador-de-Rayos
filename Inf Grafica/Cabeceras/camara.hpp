#ifndef camara_h_
#define camara_h_

#include "vector.hpp"
#include "punto.hpp"
#include "operadorVector.hpp"

class Camara {

	int resX, resY;
	float distancia;
	Punto posicion, plano;
	Vector ejes[3];

	void trazaRayos();
	
	public:

		void set_values(Punto, Vector, Vector, Vector,int resolucionX,int resolucionY, float _distancia);
		Vector getVX();
		Vector getVY();
		Vector getVZ();
		Punto getPosicion();

		void setVX(Vector);
		void setVY(Vector);
		void setVZ(Vector);
		void setPosicion(Punto);

		void trazarRayos();

		
};

#endif