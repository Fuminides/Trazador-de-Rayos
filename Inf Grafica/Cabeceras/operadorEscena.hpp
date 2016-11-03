/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet 681721
 *
 * Clase que representa a una escena.
 */
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

    
    const int NUMERO_REBOTES  = 4;
    const double AMBIENTE = 0.1;
    const double REFRACCION_MEDIO = 1;

    Camara camara;
    std :: vector < Figura * > figuras;
    std :: vector < Luz > luces;


    /**
     * Calcula la BDRF de Phong en el punto dado de la figura dada.
     */
    Color phong(Figura * figura, Punto x, Vector entrada, Vector salida, Luz);

public:

    /**
     * Incluye una camara nueva en la escena
     */
    void setCamara(Camara); 

    /*
     * Añade una esfera al array de esferas de la escena
     */
    void anyadirFigura(Figura * f);

    /*
     * Anyade una fuente de luz a la escena
     */
    void anyadirLuz(Luz);

    /**
     * Dibuja la escena
     */
    void dibujar();

    /**
     * Renderiza el color de un punto
     */
    Color renderizar(Punto c, Figura * figura, int, Punto, double);

    /**
     * Define un rebote especular y lo calcula con el numero dado de rebotes de profundidad.
     */
    Color reboteEspecular(Figura * figura, Punto origen, Vector R, int numero);

    /**
     * Calcula una refraccion especular en el punto desde el punto vista dado, con los dos coeficientes de refraccion de ambos medios.
     */
    Color refraccionEspecular(Figura * figura, Punto origen, Vector vista, double n1, double n2, int);

};

#endif
