#ifndef FILE_PLY_
#define FILE_PLY_ 

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#include "triangulo.hpp"

void leer(string, Triangulo *);
void nextLine(ifstream * istream, string * str);
Punto leerPunto(ifstream * stream);
int nTriangulos();
int nPoligonos();

#endif