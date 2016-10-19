#ifndef vector_h_
#define vector_h_

#include "objeto_cartesiano.hpp"

 class Vector : public ObjetoCartesiano {
 	
 public:
 	
 	bool esVector();

 	/**
 	 * Devuelve el modulo del vector.
 	 */
	float modulo();

	/**
	 * Normaliza el vector
	 */
	void normalizar();
 };

 #endif