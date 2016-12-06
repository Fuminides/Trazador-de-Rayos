#include "leer_ply.hpp"

void nextLine(ifstream * istream, string * str){
	while ((istream->peek()!='\n') && (*istream>>*str));
	*istream>> *str;
}

vector<Triangulo> leer(string nombre) {
  ifstream myfile;
  string word, dump;
  myfile.open (nombre);
  Punto p1, p2, p3;
  int nTr = 0;
  vector<Triangulo> v;

  do{
  	myfile >> word;
  	if (word.compare("element")){
  		myfile >> word;
  		if ( word.compare("vertex")){
  			myfile >> nTr;
  			printf("%d ", nTr);
  		}
  	}
  	nextLine(&myfile, &dump);
  } while (!word.compare("end_header"));

  p1 = leerPunto(&myfile);
  p2 = leerPunto(&myfile);
  p3 = leerPunto(&myfile);

  Triangulo tr;
  tr.set_values(p1,p2,p3);
  v.push_back(tr);


  for (int i = 0; i < nTr; ++i)
  {
  	p1 = p2;
  	p2 = p3;
  	p3 = leerPunto(&myfile);
  	tr.set_values(p1,p2,p3);
  	v.push_back(tr);
  	nextLine(&myfile,&dump);
  }
  myfile.close();

  return v;
}

Punto leerPunto(ifstream * file){
	double coord[3];
	Punto p;

	*file >> coord[0] >> coord[1] >> coord[2];
  	printf("%f\t%f\t%f\t", coord[0], coord[1], coord[2]);
  	p.set_values(coord[0], coord[1], coord[2]);
}