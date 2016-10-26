#include <iostream>

#include "objeto_cartesiano.hpp"
#include "operadorEscena.hpp"
#include "punto.hpp"
#include "vector.hpp"
#include "operadorVector.hpp"
#include "matriz.hpp"
#include "camara.hpp"

using namespace std;

int cuentaInt = 1;

int cuenta();

int main(int argc, char ** argv){
    operadorEscena escena;
    Camara camara;
    Punto origenCamara, origenLuz, origenLuz2;
    Vector vc1, vc2, vc3;
    Esfera esfera, esfera2, esfera3;
    Color rojo, blanco, azul, negro, verde;
    Luz luz1, luz2;



    rojo.set_values(255, 0, 0);
    blanco.set_values(255, 255, 255);
    azul.set_values(0,0, 255);
    negro.set_values(0,0,0);
    verde.set_values(0,255,0);

    origenCamara.set_values(1,0,0);
    vc1.set_values(1,0,0);
    vc2.set_values(0,1,0);
    vc3.set_values(0,0,1);

    esfera.setOrigen(80, 10, -20);
    esfera.setColor(negro);
    esfera.setRadio(10);
    esfera.setId(cuenta());
    esfera.setReflejo(0.5);

    esfera2.setOrigen(80, -20, -20);
    esfera2.setColor(verde);
    esfera2.setRadio(10);
    esfera2.setId(cuenta());
    esfera2.setReflejo(0.01);

    esfera3.setOrigen(0, 0, 0);
    esfera3.setColor(azul);
    esfera3.setRadio(1000000);
    esfera3.setReflejo(0.2);
    esfera3.setId(cuenta());

    origenLuz.set_values(80, 0, 20);
    luz1.set_values(origenLuz, blanco, 100);

    origenLuz2.set_values(30, 30, -20);
    luz2.set_values(origenLuz2, blanco, 50);

    camara.set_values(origenCamara, vc1, vc2, vc3, 4* 200, 4 * 200);

    
    escena.anyadirFigura(&esfera2);
    escena.anyadirFigura(&esfera);
    //escena.anyadirFigura(&esfera3);

    
    escena.anyadirLuz(luz1);   
    escena.anyadirLuz(luz2);

    escena.setCamara(camara);
    escena.dibujar();

}

int cuenta(){
    int aux = cuentaInt;
    cuentaInt++;
    return aux;
}