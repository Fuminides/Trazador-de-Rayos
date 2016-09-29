#include "punto.h"
#include "vector.h"
#include "operadorVector.h"

#include <cmath>

float multiplicacionMatricial(Vector v1, Vector v2){
	//Solo para vectores 3D!!
	return v1.getX()*v2.getX() + v1.getY()*v2.getY() + v1.getZ()*v2.getZ();
}

float productoEscalar(Vector v1, Vector v2){
	return multiplicacionMatricial(v1, v2);
}

float modulo(Vector v){
	return sqrt(pow(v.getX(), 2) + pow(v.getY(), 2) + pow(v.getZ(), 2));
}

Vector normalizar(Vector v){
	Vector vR;
	float module = modulo(v);
	vR.set_values(v.getX()/module, v.getY()/module, v.getZ()/module);
	return vR;
}

Vector productoVectorial(Vector v1, Vector v2){
	float x1 = v1.getX(), x2 = v2.getX(),
		y1 = v1.getY(), y2 = v2.getY(),
		z1 = v1.getZ(), z2 = v2.getZ(), xR, yR, zR;
	Vector vR;
	//Calculamos los determinantes (hardcodeado, solo para vectores 3d)
	xR = y1 * z2 - z1 * y2;
	yR = x1 * z2 - x2 * z1;
	zR = x1 * y2 - x2 * y1;
	vR.set_values(xR, -yR, zR);

	return vR; 
}

