#include "operadorEscena.hpp"

/*
 * Añade una esfera al array de figuras de la escena
 */
void operadorEscena::anyadirFigura(Figura * f){
    figuras.push_back(f);
}

/**
 * Traza rayos por la pantalla y detecta
 * que figuras han interceptado.
 */
void operadorEscena::dibujar(){
    Color defecto;
    int distancia, min = -1, filas = 0;
    Figura * choque;

    defecto.set_values(255,255,255);
    std::list<Rayo> rayos = camara.trazarRayos();
    std::vector<Color> pixels;
    pixels.reserve(camara.getResX() * camara.getResY());

    for ( Rayo rayo : rayos){
        //std::cout << "Rayo : " << std::to_string(rayo.getVector().getX()) <<", "<<std::to_string(rayo.getVector().getY())<<", "<<std::to_string(rayo.getVector().getZ()) << '\n';
        for ( Figura * figuraP : figuras){
            distancia = figuraP->intersectar(rayo);

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
        if ( min != -1){
            Punto puntoRender;
            Vector direccion = rayo.getVector();
            puntoRender.set_values(direccion.getX() * distancia, direccion.getY() * distancia, direccion.getZ() * distancia);
            pixels.push_back(renderizar(puntoRender, choque));
            min = -1;
        }
        else{
            pixels.push_back(defecto);
        }
    }

    //Habria que pintar el color de la figura
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

void operadorEscena::anyadirLuz(Luz l){
    luces.push_back(l);
}

Color operadorEscena::renderizar(Punto p, Figura * figura){
    Color inicial;
    inicial.set_values(0,0,0);

    for ( Luz luz: luces){
        inicial.sumar(phong(figura, p, restaPuntos(luz.getOrigen(), p),restaPuntos(camara.getPosicion(),p), luz));
    }
}

Color operadorEscena::phong(Figura * figura, Punto x, Vector luzO, Vector vista, Luz fuente){
    Vector normal, R, luz = luzO;

    Color base = figura->getColor(), colorLuz = fuente.getColor();
    float kd = 0.4, La = 0.2, n = 5, ks = 0.5;
    base.set_values(0,0,0);
    luz.normalizar();
    vista.normalizar();

    normal = figura->normal(x);
    R = restaVectores(valorPorVector(normal, 2 * productoEscalar(luz, normal)), luz);

    base.multiplicar(kd); 
    colorLuz.multiplicar(kd * productoEscalar(normal, luz) + ks * pow(productoEscalar(R, vista), n));

    base.sumar(colorLuz);

    return base;
}