/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet 681721
 *
 * Clase que representa a una estructura de aceleración Bounding Volume Box
 */
#ifndef BOUNDING_BOX_H_
#define BOUNDING_BOX_H_ 

#include <math.h>
#include <vector>

#include "punto.hpp"
#include "vector.hpp"
#include "plano.hpp"
#include "color.hpp"
#include "figura.hpp"
#include "operadorVector.hpp"

class Box : public Figura{

	std :: vector < Figura * > figuras;
	Figura * storageAux;
	Plano pl1, pl2, pl3, pl4, pl5, pl6;

	Plano crearPlano(Punto, Punto, Punto);

public:

	void set_values(Punto, Punto, Punto, Punto, Punto, Punto);

	/*
     * Devuelve la distancia a la que un Rayo r intercepta a un objeto de la bounding box f, -1 en caso de que no 
     * la intercepte
     */
    double intersectar(Rayo);

    /**
     * No hace nada en las box. (Se podria implementar, pero seria muy ineficiente. Usar el storage cen vez de esto)
     */
    Vector normal(Punto);

    /**
     * Guarda la ultima figura de la caja en una variable auxiliar.
     */
    Figura * store();

    bool isBox();


};

#endif