/**
 * Javier Fumanal Idocin 684229
 * Silvia Uson Fortanet 681721
 *
 * Clase que representa a montecarlo
 */
#ifndef MONTECARLO_H
#define MONTECARLO_H
#include "objeto_cartesiano.hpp"
#include "punto.hpp"
#include "vector.hpp"
#include "operadorVector.hpp"
#include "matriz.hpp"
class montecarlo
{
    double inclination;
    double azimuth;
    Vector x;
    Vector n;
public:
    montecarlo(double inclination,double azimuth,Vector x, Vector n);
    
    double getInclination();
    double getAzimuth();
    Vector getn();
    Vector getx();
    
    Matriz calcularT();
    Matriz inversaT(Matriz T);
        
    Vector calcularw();
    
    Vector multiplicarMatrizValores(Matriz T1,double x1, double x2,double x3);
    
    

};

#endif // MONTECARLO_H
