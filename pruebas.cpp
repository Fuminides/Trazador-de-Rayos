#include <iostream>

#include "objeto_cartesiano.h"
#include "punto.h"
#include "vector.h"
#include "operadorVector.h"

using namespace std;

int main(int argc, char ** argv){
	Punto punto;
	punto.set_values(1,2,3);
	Vector v1, v2, v3;
	v1.set_values(1,2,3);
	v2.set_values(4,5,6);
	v3 = productoVectorial(v1,v2);
	cout << "Producto Escalar: " << productoEscalar(v1,v2) << '\n';
	cout << "Producto Vectorial: (" << v3.getX() << ", " << v3.getY() << ", " << v3.getZ() << ")" << '\n';
	cout << "Modulo: " << modulo(v3) << '\n';
	v3 = normalizar(v3);
	cout << "Normalizado: (" << v3.getX() << ", " << v3.getY() << ", " << v3.getZ() << ")" << '\n';
}