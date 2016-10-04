#ifndef matriz_h_
#define matriz_h_ 

#include "vector.hpp"

class Matriz {

	Vector vectores[4];

public:

	void set_values(Vector _v1, Vector _v2, Vector _v3, Vector _v4);

	Vector getVector(int indice);
	Vector getVector1();
	Vector getVector2();
	Vector getVector3();
	Vector getVector4();

	float get(int x, int y);

	void setVector(int, Vector v);
	void setVector1(Vector v);
	void setVector2(Vector v);
	void setVector3(Vector v);
	void setVector4(Vector v);
};

#endif