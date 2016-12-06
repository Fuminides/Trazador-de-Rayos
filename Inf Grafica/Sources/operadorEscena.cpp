#include "operadorEscena.hpp"

/*
 * Añade una esfera al array de figuras de la escena
 */
void operadorEscena::anyadirFigura(Figura * f){
    if (f->isLuz()){
        std::vector<Luz> luces;
        luces = f->getLuces();
        for ( Luz luz: luces){
            anyadirLuz(luz);
        }
    }
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

    defecto.set_values(0,0,0, NORMALIZAR_COLORES);
    std::list<Rayo> rayos = camara.trazarRayos();
    
    std::vector<Color> pixels;
    pixels.reserve(camara.getPixels());
    int i = 0;
    for ( Rayo rayo : rayos){
        for ( Figura * figuraP : figuras){
            distancia = figuraP->intersectar(rayo);
            
            if ( distancia >= 0 ){
                if ( (min == -1) | (distancia < min)){
                    min = distancia;
                    choque = figuraP;
                    if (choque->isBox()){
                        choque = ((Box *) choque)->store();
                    } 
                }
            }
        }
        if ( min != -1){
            if ( !choque->isLuz() ) {
                Punto puntoRender, origenRayos = camara.getPosicion();
                Vector direccion = rayo.getVector();
                puntoRender.set_values(origenRayos.getX() + direccion.getX() * min, origenRayos.getY() + direccion.getY() * min, 
                    origenRayos.getZ() + direccion.getZ() * min);
                pixels.push_back(renderizar(puntoRender, choque, NUMERO_REBOTES, camara.getPosicion(), REFRACCION_MEDIO, true));
                min = -1;
            }
            else{
                //std::cout << "Chocamos luz\n";
                pixels.push_back(choque->getColor());
                min = -1;
            }
        }
        else{
            pixels.push_back(defecto);
        }

        i++;
        std::cout << "Renderizamos pixel numero: " << std::to_string(i) << "\n";
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

    if ( NORMALIZAR_COLORES ){
        double max = 0;
        for ( Color color : pixels){
            if ( color.max() > max) max = color.max();
        }
        cout << "max: " << to_string(max) << "\n";
        if ( max > 255){
            for ( Color color : pixels){
                color.multiplicar(1.0/max);
                color.multiplicar(255);
                cout << "Color: " << color.to_string() << "\n";
            }    
        }
    }
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

Color operadorEscena::renderizar(Punto p, Figura * figura, int numeroRebotes, Punto origenVista, double refraccionMedio, bool indirecto){
    double distancia; bool libre, libreCompleto = true;
    Color negro;
    negro.set_values(0,0,0, NORMALIZAR_COLORES);
    Color inicial = negro;
    
    double kd = AMBIENTE; 
    Vector dirLuz;
    //cout << "Chequea luces\n";
    int min;
    for ( Luz luz: luces){
        libre = true;
        Vector dirLuz = restaPuntos(luz.getOrigen(), p);
        Rayo puntoDirLuz;
        double dLuz = dirLuz.modulo();
        dirLuz.normalizar();
        puntoDirLuz.set_values(p, dirLuz);

        min = -1;

        for ( Figura * figuraP : figuras){
            distancia = figuraP->intersectar(puntoDirLuz);

            if (( (distancia > 0) && (dLuz > distancia)) && (!figuraP->isLuz())){

                if ( min == -1){
                    min = distancia;
                }
                else if (distancia < min){
                    min = distancia;
                }
            }
        }

        if ( min > -1 ){
            libre = false;
        }

        if ( libre ){
            Color auxC = figura->getColor();
            auxC.multiplicar(kd / M_PI);

            if ( figura->getBRDF() == 0) inicial.sumar(phong(figura, p, dirLuz,restaPuntos(camara.getPosicion(),p), luz)); 
            else if (figura->getBRDF() == 1) inicial.sumar(ward(restaPuntos(camara.getPosicion(),p), dirLuz, figura->normal(p), luz, p));
            inicial.sumar(auxC);

            //Caminos especulares
            if ( numeroRebotes > 0){
                Vector R;
                R = restaVectores(valorPorVector(figura->normal(p), 2 * productoEscalar(dirLuz, figura->normal(p))), dirLuz);
                Color auxC = reboteEspecular(figura, p, R, numeroRebotes);
                auxC.multiplicar(figura->getReflejo());
                inicial.sumar(auxC);

                auxC = refraccionEspecular(figura, p, restaPuntos(p, origenVista), refraccionMedio, figura->getRefraccion(), numeroRebotes);
                auxC.multiplicar (figura->getCoefRefraccion());
                inicial.sumar(auxC);
            }
        }
    }
    min = -1;
    if ( indirecto ){
            //A SILVIA:HAY QUE LLAMAR AL MONTECARLO AQUI, QUE SAQUE LOS RAYOS Y LUEGO YA CALCULAR
            Figura * choque;
            Punto auxP;
            auxP.set_values(0,0,0, NORMALIZAR_COLORES);
            Montecarlo montecarlo;
            montecarlo.set_values(1.0,1.0,restaPuntos(p,auxP), figura->normal(p), NUMERO_RAYOS_INDIRECTA);
            list<Vector> vecIndir = montecarlo.calcularw();
            for ( Vector vec : vecIndir){
                Rayo rayo;
                rayo.set_values(p, vec);
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
                   // cout << "Interseccion\n";
                    Punto puntoRender, origenRayos = camara.getPosicion();
                    Vector direccion = rayo.getVector();
                    puntoRender.set_values(origenRayos.getX() + direccion.getX() * min, origenRayos.getY() + direccion.getY() * min, 
                        origenRayos.getZ() + direccion.getZ() * min);
                    //cout << "Renderiza:" << puntoRender.to_string() << "\n";
                    Color cIndir = renderizar(puntoRender, choque, NUMERO_REBOTES, camara.getPosicion(), refraccionMedio, false);
                    //cout << "Si\n";
                    cIndir.multiplicar(K_LUZ_INDIR);
                    inicial.sumar(cIndir);

                    min = -1;
                }
            }
        }

    return inicial;
}

Color operadorEscena::phong(Figura * figura, Punto x, Vector luz, Vector vista, Luz fuente){
    Vector normal, R;
    double distancia = restaPuntos(x, fuente.getOrigen()).modulo();
    fuente.atenuar(distancia);

    Color base, colorLuz = fuente.getColor();
    double kd = AMBIENTE, La = 0.2, n = 5, ks = 0.3;
    base.set_values(0,0,0, NORMALIZAR_COLORES);
    luz.normalizar();
    vista.normalizar();

    normal = figura->normal(x);
    R = restaVectores(valorPorVector(normal, 2 * productoEscalar(luz, normal)), luz);

    double coefPhong = kd/M_PI  * productoEscalar(normal, luz) + ks * (n + 2)/(2*M_PI) * pow(productoEscalar(R, vista), n);

    if ( coefPhong < 0) colorLuz.multiplicar(-coefPhong);
    else colorLuz.multiplicar(coefPhong);

    base.sumar(colorLuz);

    return base;
}

Color operadorEscena::ward(Vector o, Vector i, Vector n, Luz fuente, Punto p){
    double distancia = restaPuntos(p, fuente.getOrigen()).modulo();
    fuente.atenuar(distancia);

    double ps = 0.75, ax = 0.20, ay = 0.10, difuso, especular, exponente;
    Vector h, x, y;
    Color color= fuente.getColor();
    i.normalizar();
    o.normalizar();
    n.normalizar();

    h = valorPorVector(sumaVectores(i,o), 1 / (sumaVectores(i,o).modulo()));
    x = productoVectorial(n, i);
    y = productoVectorial(n, x);
    exponente = -(pow(productoEscalar(h,x)/ax,2) + pow(productoEscalar(h,y)/ay,2)) / pow(productoEscalar(h, n),2);
    especular = ps / (4*M_PI*ax*ay*sqrt(productoEscalar(i,n) * productoEscalar(o, n))) * pow(M_E, exponente);
    if ( especular < 0 ) especular = -especular;
    color.multiplicar(especular);

    return color;
}

Color operadorEscena::reboteEspecular(Figura * figura, Punto origen, Vector R, int numero){
    int min = -1, distancia;
    Color defecto;
    Rayo especular;
    Figura * choque;

    especular.set_values(origen, R);
    defecto.set_values(0,0,0, NORMALIZAR_COLORES);

    for ( Figura * figuraP : figuras){
        distancia = figuraP->intersectar(especular);

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
            puntoRender.set_values(origen.getX() + direccion.getX() * min, origen.getY() + direccion.getY() * min, 
                origen.getZ() + direccion.getZ() * min);
            
            return renderizar(puntoRender, choque, numero -1, origen, figura->getRefraccion(), false);
        }
    else{
        return defecto;
    }
}

Color operadorEscena::refraccionEspecular(Figura * figura, Punto origen, Vector vista, double n1, double n2, int numeroRebotes){
    Vector normal = figura->normal(origen), refraccion;
    vista.normalizar();
    normal.normalizar();
    double cosenoAngulo1 = (productoEscalar(vista, normal) / (normal.modulo() * vista.modulo()));
    double senoCAngulo2 = pow(n1/n2,2) * (1 - pow(cosenoAngulo1,2));
    Rayo rebote;
    Color defecto;
    int distancia, min = -1;
    Figura * choque;

    defecto.set_values(0,0,0, NORMALIZAR_COLORES);

    refraccion = sumaVectores(valorPorVector(vista, n1/n2), valorPorVector(normal, (n1/n2 * cosenoAngulo1 - sqrt(1-senoCAngulo2))));
    refraccion.normalizar();

    rebote.set_values(origen, refraccion);
   
    for ( Figura * figuraP : figuras){
        distancia = figuraP->intersectar(rebote);

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
   
        puntoRender.set_values(origen.getX() + refraccion.getX() * min, origen.getY() + refraccion.getY() * min,
            origen.getZ() + refraccion.getZ() * min);
   
        return renderizar(puntoRender, choque, numeroRebotes -1, origen, n2, false);
    }
    else{
        return defecto;
    }
}