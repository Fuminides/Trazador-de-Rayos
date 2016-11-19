#include "box.hpp"

bool Box::isBox() { return false; }

Vector Box::normal(Punto p){
	Vector aux;
	return aux;
}

Figura * Box::store(){
	return storageAux;
}

Plano Box::crearPlano(Punto p1, Punto p2, Punto p3){
	Vector v1, v2, n;
	Plano pl;
	double disX, disY;

	v1 = restaPuntos(p2, p1);
	v2 = restaPuntos(p3, p1);

	n = productoVectorial(v1, v2);
	n.normalizar();

	pl.set_values(p1, n, v1, v1.modulo(), v2, v2.modulo());

	return pl;
}

void Box::set_values(Punto p1, Punto p2, Punto p3, Punto p4, Punto p5, Punto p6){
	Punto p7, p8;
	Vector v;
	double x;
	pl1 = crearPlano(p1, p2, p3);
	pl2 = crearPlano(p4, p5, p6);
	pl3 = crearPlano(p4, p1, p5);
	pl4 = crearPlano(p4, p6, p2);

	v = productoVectorial(restaPuntos(p4,p2), restaPuntos(p1,p4));
	x = restaPuntos(p5, p4).getX() / v.getX();
	p7 = sumaPuntoVector(p2, valorPorVector(v,x));

	pl5 = crearPlano(p2, p7, p3);
	pl6 = crearPlano(p5, p7, p2);
}

double Box::intersectar(Rayo r){
	Figura * choque;
	double distancia, min = -1;

	if ( pl1.intersectar(r) > 0 | pl2.intersectar(r) > 0 | pl3.intersectar(r) > 0 | pl4.intersectar(r) > 0 |
		pl5.intersectar(r) > 0 | pl6.intersectar(r) > 0){

		for ( Figura * figuraP : figuras){
            distancia = figuraP->intersectar(r);
            
            if ( distancia >= 0 ){
                if ( min == -1){
                    min = distancia;
                    choque = figuraP;
                }
                else if (distancia < min){
                    min = distancia;
                    choque = figuraP;
                }
            }
        }
        storageAux = choque;

        return distancia;
	}
	else {
		return -1;
	}
}
