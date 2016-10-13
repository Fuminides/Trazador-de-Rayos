#include "camara.hpp"


void Camara::set_values(Punto p, Vector v1, Vector v2, Vector v3, int resolucionX,
	int resolucionY, float _distancia){
	distancia = _distancia;
	resX = resolucionX;
	resY = resolucionY;
	posicion = p;
	ejes[0] = v1;
	ejes[1] = v2;
	ejes[2] = v3;

	plano = sumaPuntoVector(posicion, valorPorVector(v1, distancia));
}

Vector Camara::getVX() { return ejes[0]; }
Vector Camara::getVY() { return ejes[1]; }
Vector Camara::getVZ() { return ejes[2]; }

int Camara::getResX() { return resX; }
int Camara::getResY() { return resY; }

void Camara::setVX(Vector v) { ejes[0] = v; }
void Camara::setVY(Vector v) { ejes[1] = v; }
void Camara::setVZ(Vector v) { ejes[2] = v; }


Punto Camara::getPosicion() { return posicion; }

void Camara::setPosicion(Punto p) { 
	posicion = p; 
	plano = sumaPuntoVector(posicion, valorPorVector(ejes[0], distancia));
}


std::list<Rayo> Camara::trazarRayos(){
	int x, y;
	Punto inicial, arriba, aux;
	std::list<Rayo> rayos;

	arriba = sumaPuntoVector(plano, valorPorVector(ejes[1], (resY / 2)));
	inicial = sumaPuntoVector(arriba, -1 * valorPorVector(ejes[2], (resX / 2)));
	aux = inicial;
	for ( x = 0; x < resX; x++ ){
		for( y = 0; y < resY; y++ ){

            //Añadir el rayo a la escena , si es necesario.
            Rayo r=trazarRayo(aux);
            rayos.push_front(r);

			aux = sumaPuntoVector(aux, valorPorVector(ejes[1], 1));
		}

		inicial = sumaPuntoVector(inicial, valorPorVector(ejes[2], -1));
		aux = inicial;
	}

	return rayos;
}

Rayo Camara::trazarRayo(Punto p){
	Vector dir = restaPuntos(p, posicion);
	Rayo r;
	r.set_values(posicion, dir);
	return r;
	//std::cout << "Trazo rayo por el punto: ( " << p.getX() << ", " << p.getY() << ", " << p.getZ() << ")" << '\n';  
}

