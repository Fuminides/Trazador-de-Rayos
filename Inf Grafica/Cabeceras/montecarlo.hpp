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

#include <list>
using namespace std;
class montecarlo
{
    double inclination;
    double azimuth;
    int num;
    Vector x;
    Vector n;
    
public:
    montecarlo(double inclination,double azimuth,Vector x, Vector n,int numRayos);
    
    double getInclination();
    double getAzimuth();
    Vector getn();
    Vector getx();
    int getNum();
    
    bool esta(list<Vector> usados, Vector azar);
    list<Matriz> calcularT();
    double determinante(Matriz T);
    Matriz inversaT(Matriz T);
        
    list<Vector> calcularw();
    
    Vector multiplicarMatrizValores(Matriz T1,double x1, double x2,double x3);
    
    

};

#endif // MONTECARLO_H
