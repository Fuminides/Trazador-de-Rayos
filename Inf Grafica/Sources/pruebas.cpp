#include <iostream>

#include "objeto_cartesiano.hpp"
#include "operadorEscena.hpp"
#include "punto.hpp"
#include "vector.hpp"
#include "operadorVector.hpp"
#include "matriz.hpp"
#include "camara.hpp"
#include "plano.hpp"
#include "triangulo.hpp"
#include "leer_ply.hpp"

#include <math.h>

using namespace std;

int cuentaInt = 1;

int cuenta();

int main(int argc, char ** argv){
    int escenaID = 1;
    operadorEscena escena;
    Camara camara;
    Plano plano;
    Punto origenCamara, origenLuz, origenLuz2, origenLuz3, origenPlano;
    Vector vc1, vc2, vc3, normalPlano;
    Esfera esfera, esfera2, esfera3;
    Color rojo, blanco, azul, negro, verde, naranja, gris;
    Luz luz1, luz2, luz3;



    rojo.set_values(255, 0, 0, escena.NORMALIZAR_COLORES);
    blanco.set_values(255, 255, 255, escena.NORMALIZAR_COLORES);
    azul.set_values(0,0, 255, escena.NORMALIZAR_COLORES);
    negro.set_values(0,0,0, escena.NORMALIZAR_COLORES);
    verde.set_values(0,255,0, escena.NORMALIZAR_COLORES);
    naranja.set_values(255,128,0, escena.NORMALIZAR_COLORES);
    gris.set_values(122,122,122, escena.NORMALIZAR_COLORES);

    cout << "blanco: " << blanco.to_string() << "\n";
    cout << "naranja: " << naranja.to_string() << "\n";
    cout << "gris: " << blanco.to_string() << "\n";

    origenCamara.set_values(1,0,0);
    vc1.set_values(1,0,0);
    vc2.set_values(0,1,0);
    vc3.set_values(0,0,1);

    if ( argc > 1 ){
        escenaID = atoi(argv[1]);
    }

    if ( escenaID == 2 ){
        esfera.setOrigen(90, -10, 50);
        esfera.setColor(azul);
        esfera.setRadio(10);
        esfera.setId(cuenta());
        esfera.setReflejo(0.0);
        esfera.setRefraccion(0.2);
        esfera.setCoefRefraccion(0.2);
        esfera.setBRDF(0);

        esfera2.setOrigen(30, 20, -20);
        esfera2.setColor(verde);
        esfera2.setRadio(10);
        esfera2.setId(cuenta());
        esfera2.setReflejo(0.0);
        esfera2.setRefraccion(0.1);
        esfera2.setCoefRefraccion(0.4);
        esfera2.setBRDF(0);

        esfera3.setOrigen(100, 0, -10);
        esfera3.setColor(rojo);
        esfera3.setRadio(20);
        esfera3.setReflejo(0.0);
        esfera3.setId(cuenta());
        esfera3.setRefraccion(0);
        esfera3.setCoefRefraccion(0);
        esfera3.setBRDF(0);

        origenPlano.set_values(200, -150, -150);
        normalPlano.set_values(-1,0,0);
        plano.set_values(origenPlano, normalPlano, vc3, 500, vc2, 500);
        plano.setColor(naranja);
        plano.setReflejo(0.2);
        plano.setRefraccion(0);
        plano.setCoefRefraccion(0);

        origenLuz.set_values(0, 0, 10);
        luz1.set_values(origenLuz, blanco, 30);

        origenLuz2.set_values(65, 10, 10);
        luz2.set_values(origenLuz2, blanco, 30);

        origenLuz3.set_values(100, -50, 0);
        luz3.set_values(origenLuz3, blanco, 80);

        camara.set_values(origenCamara, vc1, vc2, vc3, 10* 100, 10 * 100,  pow(1000,2) );

        
        escena.anyadirFigura(&esfera2);
        escena.anyadirFigura(&esfera);
        escena.anyadirFigura(&esfera3);
        escena.anyadirFigura(&plano);

        
        escena.anyadirLuz(luz1);   
        escena.anyadirLuz(luz2);
        escena.anyadirLuz(luz3);

        escena.setCamara(camara);
    } 
    else if ( escenaID == 1){
       Plano planoAbajo, planoDerecha, planoIzquierda, planoFondo, planoArriba, lampara;
        Esfera esferaEspejo, esferaRefracc;
        Punto oPA, oPD, oPI, pF, pA, puntoLuz, origenC, origenLampara;
        Vector nPA, nPD, nPI, nF, nA;
        Camara camera;
        double boxLenght = 50, lamparaLen = boxLenght/3;
        Luz luzBox;

        oPA.set_values(0, 0, 0);
        oPD.set_values(0, 0, boxLenght);
        oPI.set_values(0, 0, 0);
        pF.set_values(boxLenght, 0, 0);
        pA.set_values(0, boxLenght, 0);
        origenLampara.set_values(boxLenght/2 - lamparaLen, boxLenght-2, boxLenght/2 - lamparaLen/2);

        nPA.set_values(0,-1,0);
        nPD.set_values(0,0,-1);
        nPI.set_values(0,0,1);
        nF.set_values(-1,0,0);
        nA.set_values(0,1,0);

        planoAbajo.set_values(oPA, nPA, vc1, boxLenght, vc3, boxLenght);
        planoAbajo.setColor(gris);
        planoAbajo.setReflejo(0.0);
        planoAbajo.setRefraccion(0.0);
        planoAbajo.setCoefRefraccion(0.0);
        planoAbajo.setBRDF(0);

        planoDerecha.set_values(oPD, nPD, vc2, boxLenght, valorPorVector(vc1,1) , boxLenght);
        planoDerecha.setColor(naranja);
        planoDerecha.setReflejo(0.0);
        planoDerecha.setRefraccion(0.0);
        planoDerecha.setCoefRefraccion(0.0);

        planoIzquierda.set_values(oPI, nPI, vc2, boxLenght, vc1 , boxLenght);
        planoIzquierda.setColor(verde);
        planoIzquierda.setReflejo(0.0);
        planoIzquierda.setRefraccion(0.0);
        planoIzquierda.setCoefRefraccion(0.0);

        planoFondo.set_values(pF, nF, vc2, boxLenght, vc3, boxLenght);
        planoFondo.setColor(gris);
        planoFondo.setReflejo(0.0);
        planoFondo.setRefraccion(0.0);
        planoFondo.setCoefRefraccion(0.0);

        planoArriba.set_values(pA, nA, vc1, boxLenght, vc3, boxLenght);
        planoArriba.setColor(gris);
        planoArriba.setReflejo(0.0);
        planoArriba.setRefraccion(0.0);
        planoArriba.setCoefRefraccion(0.0);

        lampara.set_values(origenLampara, nA, vc1, lamparaLen, vc3, lamparaLen);
        lampara.setReflejo(0.0);
        lampara.setRefraccion(0.0);
        lampara.setCoefRefraccion(0.0);

        puntoLuz.set_values(12.5,40,15.0);
        luzBox.set_values(puntoLuz, blanco, 20);
        lampara.setLuz(luzBox);
        //lampara.setColor(gris);

        origenC.set_values(-43.30, 25, 25);
        camera.set_values(origenC, vc1, vc2, vc3, boxLenght, boxLenght,  1000*1000);

        escena.setCamara(camera);

        //escena.anyadirLuz(luzBox);
        puntoLuz.set_values(boxLenght/2,boxLenght-2,boxLenght/2);
        luz1.set_values(puntoLuz, gris, 50);
        //escena.anyadirLuz(luz1);
        puntoLuz.set_values(15,45,17.5);
        //luz2.set_values(puntoLuz, blanco, 50);
        //escena.anyadirLuz(luz2);
        puntoLuz.set_values(15,45,12.5);
        //luz3.set_values(puntoLuz, blanco, 50);
        //escena.anyadirLuz(luz3);
        puntoLuz.set_values(12.5,45,15.0);
        //luz2.set_values(puntoLuz, rojo, 50);
        //escena.anyadirLuz(luz2);


        esfera.setOrigen(boxLenght/2, 8, boxLenght/4);
        esfera.setColor(gris);
        esfera.setRadio(8);
        esfera.setReflejo(0.3);
        esfera.setRefraccion(0.0);
        esfera.setCoefRefraccion(0.0);
        esfera.setBRDF(0);

        esfera2.setOrigen(boxLenght/3, 8, boxLenght * 0.75);
        esfera2.setColor(gris);
        esfera2.setRadio(8);
        esfera2.setReflejo(0.0);
        esfera2.setRefraccion(1.33);
        esfera2.setCoefRefraccion(0.3);
        esfera2.setBRDF(0);

        origenLuz3.set_values(boxLenght/2, boxLenght -5, boxLenght/2);
        luz3.set_values(origenLuz3, blanco, boxLenght);

        escena.anyadirLuz(luz3);
        //escena.anyadirFigura(&lampara);
        escena.anyadirFigura(&esfera);
        escena.anyadirFigura(&esfera2);
        escena.anyadirFigura(&planoArriba);
        escena.anyadirFigura(&planoFondo);
        escena.anyadirFigura(&planoIzquierda);
        escena.anyadirFigura(&planoDerecha);
        escena.anyadirFigura(&planoAbajo);
    }
    else if (escenaID == 0){
        Plano planoAbajo, planoDerecha, planoIzquierda, planoFondo, planoArriba, lampara;
        Esfera esferaEspejo, esferaRefracc;
        Punto oPA, oPD, oPI, pF, pA, puntoLuz, origenC, origenLampara;
        Vector nPA, nPD, nPI, nF, nA;
        Camara camera;
        double boxLenght = 50, lamparaLen = boxLenght/3;
        Luz luzBox;

        oPA.set_values(0, 0, 0);
        oPD.set_values(0, 0, boxLenght);
        oPI.set_values(0, 0, 0);
        pF.set_values(boxLenght, 0, 0);
        pA.set_values(0, boxLenght, 0);
        origenLampara.set_values(boxLenght/2 - lamparaLen, boxLenght-2, boxLenght/2 - lamparaLen/2);

        nPA.set_values(0,-1,0);
        nPD.set_values(0,0,-1);
        nPI.set_values(0,0,1);
        nF.set_values(-1,0,0);
        nA.set_values(0,1,0);

        planoAbajo.set_values(oPA, nPA, vc1, boxLenght, vc3, boxLenght);
        planoAbajo.setColor(gris);
        planoAbajo.setReflejo(0.0);
        planoAbajo.setRefraccion(0.0);
        planoAbajo.setCoefRefraccion(0.0);

        planoDerecha.set_values(oPD, nPD, vc2, boxLenght, valorPorVector(vc1,1) , boxLenght);
        planoDerecha.setColor(naranja);
        planoDerecha.setReflejo(0.0);
        planoDerecha.setRefraccion(0.0);
        planoDerecha.setCoefRefraccion(0.0);

        planoIzquierda.set_values(oPI, nPI, vc2, boxLenght, vc1 , boxLenght);
        planoIzquierda.setColor(verde);
        planoIzquierda.setReflejo(0.0);
        planoIzquierda.setRefraccion(0.0);
        planoIzquierda.setCoefRefraccion(0.0);

        planoFondo.set_values(pF, nF, vc2, boxLenght, vc3, boxLenght);
        planoFondo.setColor(gris);
        planoFondo.setReflejo(0.0);
        planoFondo.setRefraccion(0.0);
        planoFondo.setCoefRefraccion(0.0);

        planoArriba.set_values(pA, nA, vc1, boxLenght, vc3, boxLenght);
        planoArriba.setColor(gris);
        planoArriba.setReflejo(0.0);
        planoArriba.setRefraccion(0.0);
        planoArriba.setCoefRefraccion(0.0);

        lampara.set_values(origenLampara, nA, vc1, lamparaLen, vc3, lamparaLen);
        lampara.setReflejo(0.0);
        lampara.setRefraccion(0.0);
        lampara.setCoefRefraccion(0.0);

        puntoLuz.set_values(12.5,40,15.0);
        luzBox.set_values(puntoLuz, blanco, 10);
        lampara.setLuz(luzBox);
        //lampara.setColor(gris);

        origenC.set_values(-43.30, 25, 25);
        camera.set_values(origenC, vc1, vc2, vc3, boxLenght, boxLenght,  1000*1000);

        escena.setCamara(camera);

        //escena.anyadirLuz(luzBox);
        puntoLuz.set_values(boxLenght/2,boxLenght-2,boxLenght/2);
        luz1.set_values(puntoLuz, gris, 50);
        //escena.anyadirLuz(luz1);
        puntoLuz.set_values(15,45,17.5);
        //luz2.set_values(puntoLuz, blanco, 50);
        //escena.anyadirLuz(luz2);
        puntoLuz.set_values(15,45,12.5);
        //luz3.set_values(puntoLuz, blanco, 50);
        //escena.anyadirLuz(luz3);
        puntoLuz.set_values(12.5,45,15.0);
        //luz2.set_values(puntoLuz, rojo, 50);
        //escena.anyadirLuz(luz2);


        esfera.setOrigen(boxLenght/2, 8, boxLenght/4);
        esfera.setColor(negro);
        esfera.setRadio(8);
        esfera.setReflejo(0.15);
        esfera.setRefraccion(0.0);
        esfera.setCoefRefraccion(0.0);
        esfera.setBRDF(0);

        esfera2.setOrigen(boxLenght/3, 8, boxLenght * 0.75);
        esfera2.setColor(negro);
        esfera2.setRadio(8);
        esfera2.setReflejo(0.0);
        esfera2.setRefraccion(1.33);
        esfera2.setCoefRefraccion(0.15);
        esfera2.setBRDF(0);

        origenLuz3.set_values(boxLenght/2, boxLenght -5, boxLenght/2);
        luz3.set_values(origenLuz3, blanco, 25);

        //escena.anyadirLuz(luz3);
        escena.anyadirFigura(&lampara);
        escena.anyadirFigura(&esfera);
        escena.anyadirFigura(&esfera2);
        escena.anyadirFigura(&planoArriba);
        escena.anyadirFigura(&planoFondo);
        escena.anyadirFigura(&planoIzquierda);
        escena.anyadirFigura(&planoDerecha);
        escena.anyadirFigura(&planoAbajo);
    }
    else if ( escenaID == 3){
        Plano planoAbajo, planoArriba;
        Punto oPA, pLuz, cam, pluz2;
        Esfera esferas[5];
        Luz luz, luz2, estrellas[12];
        int fila = 0, filas = 7, radio = 10;

        for ( int i = 2; i < 5; i ++){
            esferas[i].setOrigen(fila * (radio + 5), radio + 5, (i % filas) * (radio + 10));

            esferas[i].setColor(rojo);
            esferas[i].setRadio(radio);
            esferas[i].setReflejo(0.0);
            esferas[i].setRefraccion(0.0);
            esferas[i].setCoefRefraccion(0.0);
            esferas[i].setBRDF(0);

            if (( i % filas == 0) && (i != 0)){
                fila++;
            }

            escena.anyadirFigura(&esferas[i]);
        }

        for ( int i = 0; i < 12; i++){
            Punto puntoAux;
            puntoAux.set_values(1900,((double) rand() / (RAND_MAX)) * 700, -500 + i * 100);
            estrellas[i].set_values(puntoAux, blanco, 300);

            escena.anyadirLuz(estrellas[i]);
        }

        pLuz.set_values(10, 20, 10);
        pluz2.set_values(1900, 100, 0);
        luz.set_values(pLuz, blanco, 150);
        luz2.set_values(pluz2, blanco, 1850);

        cam.set_values(-10, 5, 50);

        oPA.set_values(-50,0,-50);
        esfera.setOrigen(0,0,0);

        esfera.setColor(negro);
        esfera.setRadio(2000);
        esfera.setReflejo(0.0);
        esfera.setRefraccion(0.0);
        esfera.setCoefRefraccion(0.0);
        esfera.setBRDF(0);

        planoAbajo.set_values(oPA, vc2, vc1, 500, vc3, 500);
        planoAbajo.setColor(azul);
        planoAbajo.setReflejo(0.1);
        planoAbajo.setRefraccion(0.0);
        planoAbajo.setCoefRefraccion(0.0);
        planoAbajo.setBRDF(0);

       // escena.anyadirLuz(luz);
        escena.anyadirLuz(luz2);
        escena.anyadirFigura(&planoAbajo);
        escena.anyadirFigura(&esfera);
        

        camara.set_values(cam, vc1, vc2, vc3, 10* 100, 10 * 100,  pow(1000,2) );
        escena.setCamara(camara);
    }
    else if ( escenaID == 4 ){

        Punto p1,p2,p3,p4,p5,p6;
        Punto p21,p22,p23,p24,p25,p26;
        Punto pl1, pl2, origenLampara;

        Plano abajo, fondo, lampara;
        Box bound, bound2;
        Vector desplazamiento = vc3;
        desplazamiento.multiplicar(20);

        string fichero = "apple.ply";
        Triangulo pTr[2000], pTr2[2000];

        origenCamara.set_values(-35, 10, 10);

        p1.set_values(-10, -10, -10);
        p2.set_values(-10, 15, -10);
        p3.set_values(10, -10, -10);

        p4.set_values(-10, -10, 10);
        p5.set_values(-10, 15, 10);
        p6.set_values(10, -10, 10);

        p21.set_values(-10, -10, 10);
        p22.set_values(-10, 10, 10);
        p23.set_values(10, -10, 10);

        p24.set_values(-10, -10, 30);
        p25.set_values(-10, 10, 30);
        p26.set_values(10, -10, 30);

        pl1.set_values(-20, 0, -20);
        pl2.set_values(10, -10, -20);

        origenLampara.set_values(-35, 5, 5);

        lampara.set_values(origenLampara, vc1, vc2, 5, vc3, 5);
        lampara.setReflejo(0.0);
        lampara.setRefraccion(0.0);
        lampara.setCoefRefraccion(0.0);

        

        abajo.set_values(pl1, vc2, vc1, 100, vc3, 100);
        abajo.setColor(gris);
        abajo.setReflejo(0.0);
        abajo.setRefraccion(0.0);
        abajo.setCoefRefraccion(0.0);
        abajo.setBRDF(0);

        fondo.set_values(pl2, vc1, vc2, 100, vc3, 200);
        fondo.setColor(gris);
        fondo.setReflejo(0.0);
        fondo.setRefraccion(0.0);
        fondo.setCoefRefraccion(0.0);
        fondo.setBRDF(0);

        leer(fichero, pTr);
        leer(fichero, pTr2);

        bound.set_values(p1,p2,p3,p4,p5,p6,nPoligonos());
        bound2.set_values(p21,p22,p23,p24,p25,p26,nPoligonos());

        for (int i = 0; i<nPoligonos(); i++)
        {
            pTr[i].setColor(rojo);
            pTr[i].multiplicar(100);
            bound.meter(&(pTr[i]));

            pTr2[i].setColor(azul);
            pTr2[i].multiplicar(100);
            pTr2[i].desplazar(desplazamiento);
            pTr2[i].setBRDF(1);
            bound2.meter(&(pTr2[i]));            
        }

        escena.anyadirFigura(&bound);
        escena.anyadirFigura(&bound2);
        escena.anyadirFigura(&abajo);
        escena.anyadirFigura(&fondo);

        origenLuz3.set_values(-15, 30, 10);

        luz3.set_values(origenLuz3, blanco, 20);
        lampara.setLuz(luz3);
        //escena.anyadirLuz(luz3);
        escena.anyadirFigura(&lampara);
        camara.set_values(origenCamara, vc1, vc2, vc3, 300, 300,  pow(1000,2) );

        escena.setCamara(camara);
        escena.dibujar();

        bound.free();
        bound2.free();

        exit(1);
    }

    escena.dibujar();



}

int cuenta(){
    int aux = cuentaInt;
    cuentaInt++;
    return aux;
}