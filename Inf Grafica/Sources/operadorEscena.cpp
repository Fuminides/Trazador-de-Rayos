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
    pixels.reserve(camara.getPixels());

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
            pixels.push_back(renderizar(puntoRender, choque, NUMERO_REBOTES, camara.getPosicion(), REFRACCION_MEDIO));
            min = -1;
        }
        else{
            pixels.push_back(defecto);
        }
    }

    //Habria que pintar el color de la figura
    double areaPixel = (camara.getResX() * camara.getResY()) *1.0/ (camara.getPixels()*1.0);
    
    int fila = camara.getResX() / areaPixel,
        columna = camara.getResY() / areaPixel,
        nPixels = camara.getPixels();
    
    std::ofstream myfile;
    myfile.open ("example.ppm");
    myfile << "P6 " << std::to_string(fila) << " " << std::to_string((int) (columna)) << " 255\n";
    std::cout << "P6 " << std::to_string(fila) << " " << std::to_string((int) (columna) ) << " 255\n";

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

Color operadorEscena::renderizar(Punto p, Figura * figura, int numeroRebotes, Punto origenVista, double refraccionMedio){
    double distancia; bool libre, debug = true;
    Color inicial = figura->getColor();
    
    double kd = AMBIENTE;
    inicial.multiplicar(kd); 
    Vector dirLuz;
    
    for ( Luz luz: luces){
        libre = true;
        //if ( debug ) { std::cout << "Vamos, luz\n"; }
        Vector dirLuz = restaPuntos(luz.getOrigen(), p);
        Rayo puntoDirLuz;
        double dLuz = dirLuz.modulo();
        dirLuz.normalizar();
        puntoDirLuz.set_values(p, dirLuz);

        int min = -1;

        for ( Figura * figuraP : figuras){
            //std::cout << "Chequeando esfera..." << '\n';
            distancia = figuraP->intersectar(puntoDirLuz);

            //if ( figuraP->figuraId() == 2 ) std::cout << "Punto : " << std::to_string(p.getX()) << ", " << std::to_string(p.getY()) << ", " << std::to_string(p.getZ()) << 
            //", Vector: "  << std::to_string(dirLuz.getX()) << ", " << std::to_string(dirLuz.getY()) << ", " << std::to_string(dirLuz.getZ()) << 
            //"Distancia: " << std::to_string(distancia) << '\n';
            if ( (distancia > 0) &&  (distancia < dLuz) ){
                if ( min == -1){
                    min = distancia;
                    //choque = figuraP;
                }
                else if (distancia < min){
                    min = distancia;
                    //choque = figuraP;
                }
            }
        }

        if ( min > -1 ){
            //if (debug ) std::cout << "Misterio..."  <<"\n";
            libre = false;
            //Luz indirecta en el mas cercano
        }

        if ( libre ){
            inicial.sumar(phong(figura, p, dirLuz,restaPuntos(camara.getPosicion(),p), luz)); 
            //std::cout << "Final: " << inicial.to_string() << "\n";

                 //Caminos especulares
            if ( numeroRebotes > 0){
                Vector R;
                R = restaVectores(valorPorVector(figura->normal(p), 2 * productoEscalar(dirLuz, figura->normal(p))), dirLuz);
                Color auxC = reboteEspecular(figura, p, R, numeroRebotes);
                auxC.multiplicar(figura->getReflejo());
                inicial.sumar(auxC);

                auxC = refraccionEspecular(figura, p, restaPuntos(origenVista, p), refraccionMedio, figura->getRefraccion(), numeroRebotes);
                auxC.multiplicar (figura->getCoefRefraccion());
                inicial.sumar(auxC);
            }
        }
        else{
            //std::cout << "No phong: " << inicial.to_string() << "\n";
        }
    }

   

    //Anadimos aqui la luz indirecta

    //Transformamos a local, sampleamos 

    return inicial;
}

Color operadorEscena::phong(Figura * figura, Punto x, Vector luz, Vector vista, Luz fuente){
    Vector normal, R;
    double distancia = restaPuntos(x, fuente.getOrigen()).modulo();
    fuente.atenuar(distancia);

    Color base, colorLuz = fuente.getColor();
    double kd = AMBIENTE, La = 0.2, n = 5, ks = 0.5;
    base.set_values(0,0,0);
    luz.normalizar();
    vista.normalizar();

    //std::cout << "Luz: " << colorLuz.to_string() << "\n";

    normal = figura->normal(x);
    R = restaVectores(valorPorVector(normal, 2 * productoEscalar(luz, normal)), luz);

    //base.multiplicar(kd); 

    //std::cout << base.to_string() << "\n";
    double coefPhong = kd/M_PI  * productoEscalar(normal, luz) + ks * (n + 2)/(2*M_PI) * pow(productoEscalar(R, vista), n);

    if ( coefPhong < 0) colorLuz.multiplicar(-coefPhong);
    else colorLuz.multiplicar(coefPhong);

    //std::cout << std::to_string(kd/M_PI  * productoEscalar(normal, luz) + ks * (n + 2)/(2*M_PI) * pow(productoEscalar(R, vista), n)) << "\n";

    base.sumar(colorLuz);

    //std::cout << base.to_string() << "\n";

    return base;
}

Color operadorEscena::reboteEspecular(Figura * figura, Punto origen, Vector R, int numero){
    int min = -1, distancia;
    Color defecto;
    Rayo especular;
    Figura * choque;

    especular.set_values(origen, R);
    defecto.set_values(0,0,0);

    for ( Figura * figuraP : figuras){
        //std::cout << "Chequeando esfera..." << '\n';
        distancia = figuraP->intersectar(especular);
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
            Punto puntoRender;
            Vector direccion = especular.getVector();
            //std::cout << "Rayo : " << std::to_string(direccion.getX()) <<", "<<std::to_string(direccion.getY())<<", "<<std::to_string(direccion.getZ()) << '\n';
            //std::cout << "Pto : " << std::to_string(origenRayos.getX()) <<", "<<std::to_string(origenRayos.getY())<<", "<<std::to_string(origenRayos.getZ()) << '\n';
            //std::cout << "Dis: " << std::to_string(min) << '\n';
            puntoRender.set_values(origen.getX() + direccion.getX() * min, origen.getY() + direccion.getY() * min, 
                origen.getZ() + direccion.getZ() * min);
            //std::cout << "Pto : " << std::to_string(puntoRender.getX()) <<", "<<std::to_string(puntoRender.getY())<<", "<<std::to_string(puntoRender.getZ()) << '\n';
            return renderizar(puntoRender, choque, numero -1, origen, figura->getRefraccion());
        }
        else{
            return defecto;
        }
}

Color operadorEscena::refraccionEspecular(Figura * figura, Punto origen, Vector vista, double n1, double n2, int numeroRebotes){
    Vector normal = figura->normal(origen), refraccion = valorPorVector(normal, -1);
    double sin1 = sqrt( 1 - pow(productoEscalar(normal, vista) / (normal.modulo() * vista.modulo()),2)), sin2, cos;
    Rayo rebote;
    Color defecto;
    int distancia, min = -1; 
    Figura * choque;

    defecto.set_values(0,0,0);

    sin2 = n1 * sin1 / n2; 
    cos = sqrt(1 - pow(sin2,2));
    refraccion = valorPorVector(refraccion, 1 / cos);

    rebote.set_values(origen, refraccion);

    for ( Figura * figuraP : figuras){
        //std::cout << "Chequeando esfera..." << '\n';
        distancia = figuraP->intersectar(rebote);
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
        Punto puntoRender;
        //std::cout << "Rayo : " << std::to_string(direccion.getX()) <<", "<<std::to_string(direccion.getY())<<", "<<std::to_string(direccion.getZ()) << '\n';
        //std::cout << "Pto : " << std::to_string(origenRayos.getX()) <<", "<<std::to_string(origenRayos.getY())<<", "<<std::to_string(origenRayos.getZ()) << '\n';
        //std::cout << "Dis: " << std::to_string(min) << '\n';
        puntoRender.set_values(origen.getX() + refraccion.getX() * min, origen.getY() + refraccion.getY() * min, 
            origen.getZ() + refraccion.getZ() * min);
        //std::cout << "Pto : " << std::to_string(puntoRender.getX()) <<", "<<std::to_string(puntoRender.getY())<<", "<<std::to_string(puntoRender.getZ()) << '\n';
        return renderizar(puntoRender, choque, numeroRebotes -1, origen, n2);
    }
    else{
        return defecto;
    }
}