#include "operadorEscena.hpp"

/*
 * Devuelve la distancia a la que un Rayo r intercepta a una esfera f, -1 en caso de que no 
 * la intercepte
 */
float operadorEscena::interseccionRayoEsfera(Rayo r,Esfera a){
    //Estos calculos no estan bien , pero es por tener la idea hecha.
    Punto O= r.getOrigen();
    Vector D = r.getVector();

    Punto C= a.getOrigen();

    float OC= puntoPorPunto(O, C);
    float OO= puntoPorPunto(O, O);
    float CC= puntoPorPunto(C, C);
    float OD = puntoPorVector(O, D);
    float CD = puntoPorVector(C, D);

    float raiz=(2*OD - 2*CD) * (2*OD - 2*CD) - 4*OO*(OO+CC-2*OC-(a.getRadio()*a.getRadio()));

    std::cout << "CD: " << std::to_string(CD) << ", OD: " << std::to_string(OD) << '\n';

    if (raiz<0){
        //std::cout << "Mal puesto" << '\n';
        return -1;
    }
    else{
        raiz=sqrt(raiz);
        float p1=-(2*puntoPorVector(O, D) -2* puntoPorVector(C, D));
        float d1=(p1+raiz)/(2*OO);
        float d2=(p1-raiz)/(2*OO);
        std::cout << "Raiz, Distancias: " << std::to_string(raiz) << ", " << std::to_string(d1) << " y " << std::to_string(d2) << '\n';
        if (d1 > 0 && d1<=d2){
           return d1;
        }
        else if(d2 > 0 && d2<d1){
            return d2; 
        }
        else{
            //std::cout << "Distancia: -1 \n";
            return -1;
        }
    }
    
}

float operadorEscena::puntoPorPunto(Punto p1, Punto p2){
    return p1.getX() * p2.getX() + p1.getY() * p2.getY() + p1.getZ() * p2.getZ();
}

float operadorEscena::puntoPorVector(Punto p1, Vector p2){
    return p1.getX() * p2.getX() + p1.getY() * p2.getY() + p1.getZ() * p2.getZ(); 
}

/*
 * Añade una esfera al array de esferas de la escena
 */
void operadorEscena::anyadirEsfera(Esfera f){
    esferas.push_back(f);
   //esferas.resize(esferas.size()+1);
}

/*
 * Devuelve el numero de esferas que hay en la escena
 */
int operadorEscena::tamEsferas(){
    return esferas.size();
}

/**
 * Traza rayos por la pantalla y detecta
 * que esferas han interceptado.
 */
void operadorEscena::dibujar(){
    Color defecto;
    int distancia, min = -1, filas = 0;
    Esfera choque;

    defecto.set_values(255,255,255);
    std::list<Rayo> rayos = camara.trazarRayos();
    std::vector<Color> pixels;
    pixels.reserve(camara.getResX() * camara.getResY());

    for ( Rayo rayo : rayos){
        std::cout << "Rayo : " << std::to_string(rayo.getVector().getX()) <<", "<<std::to_string(rayo.getVector().getY())<<", "<<std::to_string(rayo.getVector().getZ()) << '\n';
        for ( Esfera esfera : esferas){
            distancia = interseccionRayoEsfera(rayo, esfera);

            if ( distancia >= 0 ){
                if ( min == -1){
                    min = distancia;
                    choque = esfera;
                }
                else if (distancia < min){
                    min = distancia;
                    choque =esfera;
                }
            }
        }
        if ( min != -1){
            pixels.push_back(choque.getColor());
            min = -1;
        }
        else{
            pixels.push_back(defecto);
        }
    }

    //Habria que pintar el color de la esfera
   int fila = camara.getResX(),
    columna = camara.getResY();
    std::ofstream myfile;
    myfile.open ("example.ppm");
    myfile << "P6 " << std::to_string(fila) << " " << std::to_string(columna) << " 255\n";
    for ( Color color : pixels){
        myfile << color.splashR();
        myfile << color.splashG();
        myfile << color.splashB();
    }

    myfile.close();
}

void operadorEscena::setCamara(Camara c){
    camara = c;
}

//DEPRECATED
/*
 * Devuelve el numero de rayos que hay en la escena
 */
int operadorEscena::tamRayos(){
    return rayos.size();
    
}

/*
 * Añade un rayo al array de rayos de la escena
 */
void operadorEscena::anyadirRayo(Rayo r){
    rayos.push_back(r);
    rayos.resize(rayos.size()+1);
}

/*
 * Devuelve la distancia de la esfera mas cercana con la que intercepta un rayo r
 */
float operadorEscena::intersecciones(Rayo r){
    float menor=0;
    Esfera eMenor;
    for (int i=0; i< esferas.size();i++){
        Esfera a=esferas[i];
        float d= interseccionRayoEsfera(r,a);
        if (d>0 && d< menor){
            eMenor=a;
            menor=d;
        }
    }
    return menor;
    
}
