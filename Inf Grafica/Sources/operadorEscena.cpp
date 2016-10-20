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

    defecto.set_values(0,0,0);
    std::list<Rayo> rayos = camara.trazarRayos();
    std::vector<Color> pixels;
    pixels.reserve(camara.getResX() * camara.getResY());

    for ( Rayo rayo : rayos){
        //std::cout << "Rayo : " << std::to_string(rayo.getVector().getX()) <<", "<<std::to_string(rayo.getVector().getY())<<", "<<std::to_string(rayo.getVector().getZ()) << '\n';
        for ( Figura * figuraP : figuras){
            //std::cout << "Chequeando esfera..." << '\n';
            distancia = figuraP->intersectar(rayo);
            //std::cout << "Distancia " << std::to_string(distancia) << '\n';
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
            Punto puntoRender, origenRayos = camara.getPosicion();
            Vector direccion = rayo.getVector();
            //std::cout << "Rayo : " << std::to_string(direccion.getX()) <<", "<<std::to_string(direccion.getY())<<", "<<std::to_string(direccion.getZ()) << '\n';
            //std::cout << "Pto : " << std::to_string(origenRayos.getX()) <<", "<<std::to_string(origenRayos.getY())<<", "<<std::to_string(origenRayos.getZ()) << '\n';
            //std::cout << "Dis: " << std::to_string(min) << '\n';
            puntoRender.set_values(origenRayos.getX() + direccion.getX() * min, origenRayos.getY() + direccion.getY() * min, 
                origenRayos.getZ() + direccion.getZ() * min);
            //std::cout << "Pto : " << std::to_string(puntoRender.getX()) <<", "<<std::to_string(puntoRender.getY())<<", "<<std::to_string(puntoRender.getZ()) << '\n';
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
        //if ( (color.splashR() != 0) ) std::cout << "\nColor escrito: " << color.to_string();
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
    double distancia; bool libre = true, debug = false;
    Color inicial = figura->getColor();
    //if ( inicial.splashB() == 255 ){ std::cout << "Interesante\n"; debug = true; }
    inicial.multiplicar(0.4); 
    

    for ( Luz luz: luces){
        //if ( debug ) { std::cout << "Vamos, luz\n"; }
        Vector dirLuz = restaPuntos(luz.getOrigen(), p);
        Rayo puntoDirLuz;
        puntoDirLuz.set_values(p, dirLuz);

        distancia = figura->intersectar(puntoDirLuz);
        //if ( debug )std::cout << "Punto : " << std::to_string(p.getX()) << ", " << std::to_string(p.getY()) << ", " << std::to_string(p.getZ()) << 
        //", Vector: "  << std::to_string(dirLuz.getX()) << ", " << std::to_string(dirLuz.getY()) << ", " << std::to_string(dirLuz.getZ()) << ", Distancia: " << std::to_string(distancia) << '\n';

        if ( distancia >= 0 ){
            //if (debug ) std::cout << "Misterio..."  <<"\n";
            libre = false;
            //Luz indirecta en el mas cercano
        }

        if ( libre ){
            inicial.sumar(phong(figura, p, dirLuz,restaPuntos(camara.getPosicion(),p), luz)); 
            //std::cout << "Final: " << inicial.to_string() << "\n";
        }
        else{
            //std::cout << "No phong: " << inicial.to_string() << "\n";
        }

    }

    return inicial;
}

Color operadorEscena::phong(Figura * figura, Punto x, Vector luz, Vector vista, Luz fuente){
    Vector normal, R;
    double distancia = restaPuntos(x, fuente.getOrigen()).modulo();
    fuente.atenuar(distancia);

    Color base, colorLuz = fuente.getColor();
    double kd = 0.4, La = 0.2, n = 5, ks = 0.5;
    base.set_values(0,0,0);
    luz.normalizar();
    vista.normalizar();

    //std::cout << "Luz: " << colorLuz.to_string() << "\n";

    normal = figura->normal(x);
    R = restaVectores(valorPorVector(normal, 2 * productoEscalar(luz, normal)), luz);

    //base.multiplicar(kd); 

    //std::cout << base.to_string() << "\n";

    colorLuz.multiplicar(kd/M_PI  * productoEscalar(normal, luz) + ks * (n + 2)/(2*M_PI) * pow(productoEscalar(R, vista), n));
    //std::cout << colorLuz.to_string() << "\n";

    base.sumar(colorLuz);

    //std::cout << base.to_string() << "\n";

    return base;
}