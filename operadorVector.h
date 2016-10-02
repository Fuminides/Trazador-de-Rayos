#ifndef operadorVector_h_
#define operadorVector_h_ 

#include "vector.h"
#include "punto.h"

float multiplicacionMatricial(Vector v1, Vector v2);
float productoEscalar(Vector v1, Vector v2);
float modulo(Vector v);
Vector productoVectorial(Vector v1, Vector v2);
Vector normalizar(Vector v);
Vector sumaVectores(Vector v1,Vector v2);
Vector restaVectores(Vector v1,Vector v2);
Vector restaPuntos(Punto p1,Punto p2);
Vector valoxVector(Vector v,float alfa);
Punto sumaPuntoVector(Punto p,Vector v);
#endif