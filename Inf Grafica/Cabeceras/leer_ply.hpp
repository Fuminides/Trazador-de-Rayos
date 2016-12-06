#ifndef FILE_PLY_
#define FILE_PLY_ 

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#include "triangulo.hpp"

vector<Triangulo> leer(string);
void nextLine(ifstream * istream, string * str);
Punto leerPunto(ifstream * stream);

#endif