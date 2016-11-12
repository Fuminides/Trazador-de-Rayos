#include "figura.hpp"

Color Figura::getColor(){ return color; }

int Figura::figuraId() { return idFigura; }

double Figura::getReflejo(){ return reflejo; }

double Figura::getRefraccion(){	return refraccion; }

double Figura::getCoefRefraccion(){ return kr; }

int Figura::getBRDF(){ return brdf; }

void Figura::setColor(Color c){ color = c; }

void Figura::setReflejo(double r){ reflejo = r; }

void Figura::setRefraccion(double r){ refraccion = r; }

void Figura::setCoefRefraccion(double r){ kr = r; }

void Figura::setId(int id) { idFigura = id; }

void Figura::setBRDF(int b){ brdf = b; }