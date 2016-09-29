#include <iostream>

#include "objeto_cartesiano.h"
#include "punto.h"
#include "vector.h"
using namespace std;

int main(int argc, char ** argv){
	Punto punto;
	punto.set_values(1,2,3);
	cout << punto.getY() << '\n';
}