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
    Punto origenCamara;
    Vector vc1, vc2, vc3;
    Esfera esfera;
    Color rojo;

    rojo.set_values(100, 0, 0);

    origenCamara.set_values(1,0,0);
    vc1.set_values(1,0,0);
    vc2.set_values(0,1,0);
    vc3.set_values(0,0,1);

    esfera.setOrigen(15, 10, 30);
    esfera.setColor(rojo);
    esfera.setRadio(5);

    camara.set_values(origenCamara, vc1, vc2, vc3, 16* 10, 9 * 10, 15);

    escena.anyadirEsfera(esfera);
    escena.setCamara(camara);
    escena.dibujar();

}