#ifndef OPERADORESCENA_H
#define OPERADORESCENA_H

#include "rayo.hpp"
#include "esfera.hpp"
#include "camara.hpp"

#include <vector>
#include <string>
#include <list>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>

class operadorEscena
{
private:

    Camara camara;
    std :: vector < Esfera > esferas;
    std :: vector < Rayo > rayos;
public:

    /**
     * Incluye una camara nueva en la escena
     */
    void setCamara(Camara); 
    /*
     * Devuelve la distancia a la que un Rayo r intercepta a una esfera f, -1 en caso de que no 
     * la intercepte
     */
     float interseccionRayoEsfera(Rayo r,Esfera a);
     /*
     * Devuelve la distancia de la esfera mas cercana con la que intercepta un rayo r
     */
    float intersecciones(Rayo r);
    /*
     * Añade un rayo al array de rayos de la escena
     */
    void anyadirRayo(Rayo r);
    /*
     * Añade una esfera al array de esferas de la escena
     */
    void anyadirEsfera(Esfera f);
    /*
     * Devuelve el numero de rayos que hay en la escena
     */
    int tamEsferas();
    /*
     * Devuelve el numero de esferas que hay en la escena
     */
    int tamRayos();

    void dibujar();

};

#endif
