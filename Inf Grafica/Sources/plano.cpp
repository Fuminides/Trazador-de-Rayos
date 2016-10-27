#include "plano.hpp"

void Plano::set_values(Punto o, Vector n, Vector _vy, double alt, Vector _vx, double anch){
	origen = o;
	vNormal = n;
	altura = alt;
	anchura = anch;
	vectorX = _vx;
	vectorX.normalizar();
	vectorY = _vy;
	vectorY.normalizar();

	double a = vNormal.getX(), b = vNormal.getY(), c = vNormal.getZ(),
	px = origen.getX(), py = origen.getY(), pz = origen.getZ();

	d = a*px + b*py + c*pz;

}

double Plano::intersectar(Rayo r){
	double a = vNormal.getX(), b = vNormal.getY(), c = vNormal.getZ(),
	ox = r.getOrigen().getX(), oy = r.getOrigen().getY(), oz = r.getOrigen().getZ(),
	vx = r.getVector().getX(), vy = r.getVector().getY(), vz = r.getVector().getZ(), t;
	Punto aux;
	Vector proyectar;
	t = -(a*ox+b*oy + c*oz-d)/(a*vx+b*vy+c*vz);

	vx = vx * t + ox;
	vy = vy * t + oy;
	vz = vz * t + oz;

	aux.set_values(vx, vy,vz);
	proyectar = restaPuntos(aux, origen);

	if ( productoEscalar(proyectar, vectorX) > anchura) {
		t = -1;
	}
	else if ( productoEscalar(proyectar, vectorY) > altura ){
		t = -1;
	}

	return t;
}

Vector Plano::normal(Punto p){
	return vNormal;
}