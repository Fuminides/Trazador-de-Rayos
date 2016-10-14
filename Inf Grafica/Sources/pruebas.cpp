#include <iostream>

#include "objeto_cartesiano.hpp"
#include "operadorEscena.hpp"
#include "punto.hpp"
#include "vector.hpp"
#include "operadorVector.hpp"
#include "matriz.hpp"
#include "camara.hpp"

using namespace std;

int main(int argc, char ** argv){
	/*Punto punto;
	punto.set_values(1,2,3);
    Punto punto1;
	punto1.set_values(4,5,6);
	Vector v1, v2, v3,v4;
	v1.set_values(1,2,3);
	v2.set_values(4,5,6);
	v3 = productoVectorial(v1,v2);
	cout << "Producto Escalar: " << productoEscalar(v1,v2) << '\n';
	cout << "Producto Vectorial: (" << v3.getX() << ", " << v3.getY() << ", " << v3.getZ() << ")" << '\n';
	cout << "Modulo: " << v3.modulo() << '\n';
	v3.normalizar();
	cout << "Normalizado: (" << v3.getX() << ", " << v3.getY() << ", " << v3.getZ() << ")" << '\n';
    v4=sumaVectores(v1,v2);
    cout << "Suma de v1 y v2: (" << v4.getX() << ", " << v4.getY() << ", " << v4.getZ() << ")" << '\n';
    v4=restaVectores(v1,v2);
    cout << "Resta de v1 y v2: (" << v4.getX() << ", " << v4.getY() << ", " << v4.getZ() << ")" << '\n';
    Vector v5=restaPuntos(punto,punto1);
    cout << "Resta de p1 y p2: (" << v5.getX() << ", " << v5.getY() << ", " << v5.getZ() << ")" << '\n';
    Vector v6;
    v6=valorPorVector(v2,3);
    cout << "v2 por 3: (" << v6.getX() << ", " << v6.getY() << ", " << v6.getZ() << ")" << '\n';
    Punto p1;
    p1=sumaPuntoVector(punto,v2);
    cout << "v2 + punto (" << p1.getX() << ", " << p1.getY() << ", " << p1.getZ() << ")" << '\n';
    Matriz m;
    m.set_values(v1,v2,v3);
    cout << m.toString() << '\n';
    v6 = matrizPorVector(m, v6);
    cout << "v6 por matriz: (" << v6.getX() << ", " << v6.getY() << ", " << v6.getZ() << ")" << '\n';
    Punto p;
    Camara c;
    Vector u1, u2, u3;

    p.set_values(0,0,0);
    u1.set_values(1.0,0,0);
    u2.set_values(0,1.0,0);
    u3.set_values(0,0,1.0);
    c.set_values(p, u1, u2, u3, 16, 9, 10);
    c.trazarRayos();*/

    operadorEscena escena;
    Camara camara;
    Punto origenCamara;
    Vector vc1, vc2, vc3;
    Esfera esfera;
    Color rojo;

    rojo.set_values(100, 0, 0);

    origenCamara.set_values(0,0,0);
    vc1.set_values(1,0,0);
    vc2.set_values(0,1,0);
    vc3.set_values(0,0,1);

    esfera.setOrigen(70, 0 , 0);
    esfera.setColor(rojo);
    esfera.setRadio(50);

    camara.set_values(origenCamara, vc1, vc2, vc3, 160* 2, 90 * 2, 50);

    //escena.anyadirEsfera(esfera);
    escena.setCamara(camara);

    escena.dibujar();
}