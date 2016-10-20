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
    operadorEscena escena;
    Camara camara;
    Punto origenCamara, origenLuz, origenLuz2;
    Vector vc1, vc2, vc3;
    Esfera esfera, esfera2;
    Color rojo, blanco, verde;
    Luz luz1, luz2;



    rojo.set_values(255, 0, 0);
    blanco.set_values(255, 255, 255);
    verde.set_values(0,255, 255);

    origenCamara.set_values(1,0,0);
    vc1.set_values(1,0,0);
    vc2.set_values(0,1,0);
    vc3.set_values(0,0,1);

    esfera.setOrigen(100, 10, 30);
    esfera.setColor(rojo);
    esfera.setRadio(10);

    esfera2.setOrigen(80, 45, 15);
    esfera2.setColor(verde);
    esfera2.setRadio(10);

    origenLuz.set_values(100, 0, -45);
    luz1.set_values(origenLuz, blanco, 100);

    origenLuz2.set_values(100, 50, -45);
    luz2.set_values(origenLuz2, verde, 100);

    camara.set_values(origenCamara, vc1, vc2, vc3, 4* 300, 4 * 300);

    escena.anyadirFigura(&esfera2);
    escena.anyadirFigura(&esfera);

    escena.anyadirLuz(luz1);
    escena.anyadirLuz(luz2);
    escena.setCamara(camara);
    escena.dibujar();

}