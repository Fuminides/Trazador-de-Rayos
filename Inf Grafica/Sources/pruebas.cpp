#include <iostream>

#include "objeto_cartesiano.hpp"
#include "operadorEscena.hpp"
#include "punto.hpp"
#include "vector.hpp"
#include "operadorVector.hpp"
#include "matriz.hpp"
#include "camara.hpp"
#include "plano.hpp"

#include <math.h>

using namespace std;

int cuentaInt = 1;

int cuenta();

int main(int argc, char ** argv){
    operadorEscena escena;
    Camara camara;
    Plano plano;
    Punto origenCamara, origenLuz, origenLuz2, origenLuz3, origenPlano;
    Vector vc1, vc2, vc3, normalPlano;
    Esfera esfera, esfera2, esfera3;
    Color rojo, blanco, azul, negro, verde, naranja;
    Luz luz1, luz2, luz3;



    rojo.set_values(255, 0, 0);
    blanco.set_values(255, 255, 255);
    azul.set_values(0,0, 255);
    negro.set_values(0,0,0);
    verde.set_values(0,255,0);
    naranja.set_values(255,128,0);

    origenCamara.set_values(1,0,0);
    vc1.set_values(1,0,0);
    vc2.set_values(0,1,0);
    vc3.set_values(0,0,1);

    esfera.setOrigen(90, -10, 40);
    esfera.setColor(azul);
    esfera.setRadio(10);
    esfera.setId(cuenta());
    esfera.setReflejo(0);
    esfera.setRefraccion(0);

    esfera2.setOrigen(30, 20, -20);
    esfera2.setColor(verde);
    esfera2.setRadio(10);
    esfera2.setId(cuenta());
    esfera2.setReflejo(0.0);
    esfera2.setRefraccion(0.1);
    esfera2.setCoefRefraccion(0.4);

    esfera3.setOrigen(100, 0, -10);
    esfera3.setColor(rojo);
    esfera3.setRadio(20);
    esfera3.setReflejo(0.05);
    esfera3.setId(cuenta());
    esfera3.setRefraccion(0.2);
    esfera3.setCoefRefraccion(0.6);
    esfera3.setBRDF(1);

    origenPlano.set_values(300, -50, -50);
    normalPlano.set_values(-1,0,0);
    plano.set_values(origenPlano, normalPlano, vc3, 200, vc2, 200);
    plano.setColor(naranja);
    plano.setReflejo(0.5);
    plano.setRefraccion(0);
    plano.setCoefRefraccion(0);

    origenLuz.set_values(0, 0, 10);
    luz1.set_values(origenLuz, blanco, 50);

    origenLuz2.set_values(65, 10, 10);
    luz2.set_values(origenLuz2, blanco, 50);

    origenLuz3.set_values(100, -50, 0);
    luz3.set_values(origenLuz2, blanco, 50);

    camara.set_values(origenCamara, vc1, vc2, vc3, 10* 100, 10 * 100,  pow(10* 100,2) );

    
    escena.anyadirFigura(&esfera2);
    escena.anyadirFigura(&esfera);
    escena.anyadirFigura(&esfera3);
    escena.anyadirFigura(&plano);

    
    escena.anyadirLuz(luz1);   
    escena.anyadirLuz(luz2);
    escena.anyadirLuz(luz3);


    escena.setCamara(camara);
    escena.dibujar();

}

int cuenta(){
    int aux = cuentaInt;
    cuentaInt++;
    return aux;
}