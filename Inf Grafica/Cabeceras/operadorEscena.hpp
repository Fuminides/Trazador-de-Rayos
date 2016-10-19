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
    std :: vector < Figura * > figuras;
    std :: vector < Luz > luces;


    /**
     * Calcula la BDRF de Phong
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
    Color renderizar(Punto c, Figura * figura);




    

};

#endif
