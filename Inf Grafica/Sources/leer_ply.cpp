#include "leer_ply.hpp"
int triangles2;
int poligonos;

void nextLine(ifstream * istream, string * str){
	while ((istream->peek()!='\n') && (*istream>>*str));
}

void leer(string nombre, Triangulo * v) {
  ifstream myfile;
  string word, dump;
  myfile.open (nombre);
  Punto p1, p2, p3;
  int nTr = 0;

  do{
  	myfile >> word;
  	if (word.compare("element") == 0){
  		myfile >> word;
  		if ( word.compare("vertex")==0){
  			myfile >> nTr;
  			cout << "Triangulos: " << nTr << "\n";
  		}
      if ( word.compare("face")==0){
        myfile >> poligonos;
        cout << "Poligonos: " << poligonos << "\n";
      }
  	}
  	nextLine(&myfile, &dump);
  } while ((!word.compare("end_header")==0) && myfile.good());
  cout << "Cabecera\n";
  p1 = leerPunto(&myfile);
  p2 = leerPunto(&myfile);
  p3 = leerPunto(&myfile);

  v[0].set_values(p2,p3,p1);

  for (int i = 1; i < nTr-2; ++i)
  {
  	p1 = p2;
  	p2 = p3;
  	p3 = leerPunto(&myfile);
  	v[i].set_values(p2,p3,p1);
  }
  triangles2 = nTr - 2;

  p1 = leerPunto(&myfile);
  p2 = leerPunto(&myfile);
  p3 = leerPunto(&myfile);

  v[triangles2].set_values(p2,p3,p1);

  for (int i = triangles2 + 1; i < poligonos + triangles2; ++i)
  {
    p1 = p2;
    p2 = p3;
    myfile >> dump;
    p3 = leerPunto(&myfile);
    v[i].set_values(p2,p3,p1);
  }

  myfile.close();
}

Punto leerPunto(ifstream * file){
	double coord[3];
	Punto p;

	*file >> coord[0] >> coord[1] >> coord[2];
  	//cout << "Puntos: " << coord[0] << ","<< coord[1]<<","<< coord[2] << "\n";
  	p.set_values(coord[0], coord[1], coord[2]);

  	return p;
}

int nTriangulos(){ return triangles2; }
int nPoligonos() { return poligonos; }