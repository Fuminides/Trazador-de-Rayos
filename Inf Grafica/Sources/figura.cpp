#include "figura.hpp"

void Figura::setColor(Color c){
        color = c;
    }

    Color Figura::getColor(){
        return color;
    }

int Figura::figuraId() { return idFigura; }

double Figura::getReflejo(){
	return reflejo;
}

double Figura::getRefraccion(){
	return refraccion;
}

double Figura::getCoefRefraccion(){
	return kr;
}

void Figura::setReflejo(double r){
	reflejo = r;
}

void Figura::setRefraccion(double r){
	refraccion = r;
}

void Figura::setCoefRefraccion(double r){
	kr = r;
}

void Figura::setId(int id) { idFigura = id; }

