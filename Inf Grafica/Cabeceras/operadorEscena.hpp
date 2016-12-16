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
#include "box.hpp"
#include "montecarlo.hpp"

#include <vector>
#include <string>
#include <list>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <cmath> 

class operadorEscena
{
    
private:

    
    const int NUMERO_REBOTES  = 4;
    const double AMBIENTE = 1;
    const double REFRACCION_MEDIO = 1;
    const int NUMERO_RAYOS_INDIRECTA = 32, PATH_LEN = 5;
    const double K_LUZ_INDIR = 1.0 / (NUMERO_RAYOS_INDIRECTA + 0.000001), K_LUZ_DIR = 1;
    const bool PATH_TRACING = true;

    Camara camara;
    std :: vector < Figura * > figuras;
    std :: vector < Luz > luces;


    /**
     * Calcula la BDRF de Phong en el punto dado de la figura dada.
     */
    double phong(Figura * figura, Punto x, Vector entrada, Vector salida);

     /**
     * Calcula la BDRF de Ward en el punto dado de la figura dada.
     */
    double ward(Vector o, Vector i, Vector n, Punto x);

    double interseccion(Rayo r, Figura ** choque);

public:

    const bool NORMALIZAR_COLORES = false; //Solo funciona bien en alguna escena muy concreta con muchos fogonazos

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
    Color renderizar(Punto c, Figura * figura, int, Punto, double, bool, int);

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
