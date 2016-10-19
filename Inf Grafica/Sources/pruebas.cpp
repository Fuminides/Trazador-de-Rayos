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
    Punto origenCamara, origenLuz;
    Vector vc1, vc2, vc3;
    Esfera esfera;
    Color rojo;
    Luz luz1;

    origenLuz.set_values(50, 50, 0);

    rojo.set_values(100, 10, 10);

    origenCamara.set_values(1,0,0);
    vc1.set_values(1,0,0);
    vc2.set_values(0,1,0);
    vc3.set_values(0,0,1);

    esfera.setOrigen(100, 30, 10);
    //esfera.setColor(rojo);
    esfera.setRadio(10);

    luz1.set_values(origenLuz, rojo);

    camara.set_values(origenCamara, vc1, vc2, vc3, 4* 300, 4 * 300);

    escena.anyadirFigura(&esfera);
    escena.anyadirLuz(luz1);
    escena.setCamara(camara);
    escena.dibujar();

}