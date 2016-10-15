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

float Camara::getDistancia(){ return distancia; }


std::list<Rayo> Camara::trazarRayos(){
	int x, y;
	Punto inicial, arriba, aux;
	std::list<Rayo> rayos;

	arriba = sumaPuntoVector(plano, valorPorVector(ejes[1], (resY / 2)));
	inicial = sumaPuntoVector(arriba, valorPorVector(ejes[2],  -1 * (resX / 2)));
	aux = inicial;
	for ( x = 0; x < resY; x++ ){
		for( y = 0; y < resX; y++ ){
			//std::cout << "Inicio : " << std::to_string(aux.getX()) <<", "<<std::to_string(aux.getY())<<", "<<std::to_string(aux.getZ()) << '\n';
            //Añadir el rayo a la escena , si es necesario.
            Rayo r=trazarRayo(aux);
            rayos.push_back(r);

			aux = sumaPuntoVector(aux, valorPorVector(ejes[2], 1));
		}

		inicial = sumaPuntoVector(inicial, valorPorVector(ejes[1], -1));
		aux = inicial;
	}

	return rayos;
}

Rayo Camara::trazarRayo(Punto p){
	//std::cout << "Rest : " << std::to_string(p.getX()) <<", "<<std::to_string(p.getY())<<", "<<std::to_string(p.getZ()) << '\n';
	//std::cout << "Org : " << std::to_string(posicion.getX()) <<", "<<std::to_string(posicion.getY())<<", "<<std::to_string(posicion.getZ()) << '\n';
	Vector dir = restaPuntos(p, posicion);
	//std::cout << "Rayo : " << std::to_string(dir.getX()) <<", "<<std::to_string(dir.getY())<<", "<<std::to_string(dir.getZ()) << '\n';
	dir.normalizar();
	Rayo r;
	r.set_values(posicion, dir);
	return r;
}

