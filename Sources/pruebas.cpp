#include <iostream>

#include "objeto_cartesiano.hpp"
#include "punto.hpp"
#include "vector.hpp"
#include "operadorVector.hpp"

using namespace std;

int main(int argc, char ** argv){
	Punto punto;
	punto.set_values(1,2,3);
    Punto punto1;
	punto1.set_values(4,5,6);
	Vector v1, v2, v3,v4;
	v1.set_values(1,2,3);
	v2.set_values(4,5,6);
	v3 = productoVectorial(v1,v2);
	cout << "Producto Escalar: " << productoEscalar(v1,v2) << '\n';
	cout << "Producto Vectorial: (" << v3.getX() << ", " << v3.getY() << ", " << v3.getZ() << ")" << '\n';
	cout << "Modulo: " << modulo(v3) << '\n';
	v3 = normalizar(v3);
	cout << "Normalizado: (" << v3.getX() << ", " << v3.getY() << ", " << v3.getZ() << ")" << '\n';
    v4=sumaVectores(v1,v2);
    cout << "Suma de v1 y v2: (" << v4.getX() << ", " << v4.getY() << ", " << v4.getZ() << ")" << '\n';
    v4=restaVectores(v1,v2);
    cout << "Resta de v1 y v2: (" << v4.getX() << ", " << v4.getY() << ", " << v4.getZ() << ")" << '\n';
    Vector v5=restaPuntos(punto,punto1);
    cout << "Resta de p1 y p2: (" << v5.getX() << ", " << v5.getY() << ", " << v5.getZ() << ")" << '\n';
    Vector v6;
    v6=valoxVector(v2,3);
    cout << "v2 por 3: (" << v6.getX() << ", " << v6.getY() << ", " << v6.getZ() << ")" << '\n';
    Punto p1;
    p1=sumaPuntoVector(punto,v2);
    cout << "v2 + punto (" << p1.getX() << ", " << p1.getY() << ", " << p1.getZ() << ")" << '\n';
}