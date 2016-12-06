#ifndef TRIANGULO_H_
#define TRIANGULO_H_

#include "figura.hpp"
#include "operadorVector.hpp"
#include "plano.hpp"

#include <string>

#define EPSILON 0.000001

class Triangulo : public Figura {

	Punto p1, p2, p3;
	Vector v1, v2, v3;
	Luz foco;

	public:

		void set_values(Punto, Punto, Punto);

		double intersectar(Rayo);

		Vector normal(Punto);

		std::vector<Luz> getLuces();
		void setLuz(Luz);

		std::string to_string();

};

#endif