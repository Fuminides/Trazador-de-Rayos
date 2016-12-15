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
        min = interseccion(rayo, &choque);

        if ( min != -1){
            if ( !choque->isLuz() ) {
                Punto puntoRender, origenRayos = camara.getPosicion();
                Vector direccion = rayo.getVector();
                puntoRender.set_values(origenRayos.getX() + direccion.getX() * min, origenRayos.getY() + direccion.getY() * min, 
                    origenRayos.getZ() + direccion.getZ() * min);
                pixels.push_back(renderizar(puntoRender, choque, NUMERO_REBOTES, camara.getPosicion(), REFRACCION_MEDIO, true, PATH_LEN));
                min = -1;
            }
            else{
                Color colorAux = choque->getColor();
                pixels.push_back(colorAux);
                min = -1;
            }
        }
        else{
            pixels.push_back(defecto);
        }

        i++;
    }

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

Color operadorEscena::renderizar(Punto p, Figura * figura, int numeroRebotes, Punto origenVista, double refraccionMedio, bool indirecto, int seguirCamino){
    double distancia; bool libre, libreCompleto = true;
    Color negro;
    negro.set_values(0,0,0, NORMALIZAR_COLORES);
    Color inicial = negro;
    double kd = AMBIENTE; 
    Vector dirLuz;
    double bdrf;
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

        //min = interseccion(puntoDirLuz, &choque);
        for (int i = 0; i < figuras.size(); i++){
            distancia = figuras[i]->intersectar(puntoDirLuz);
            //cout << "Legal " << std::to_string(i) << "\n";

            if ( distancia >= 0 ){
                if ((( min == -1) | (distancia < min)) && (!figuras[i]->isLuz()) && (distancia < dLuz)){
                    min = distancia;
                }
            }
        }

        if ( min > -1 ){
            libre = false;
            //cout << "Vaya\n";
        }

        if ( libre ){
            Color auxC = figura->getColor();
            auxC.multiplicar(AMBIENTE/M_PI);
            luz.atenuar(restaPuntos(p, luz.getOrigen()).modulo());

            if ( luz.getColor().max() > 0.00001){

                if ( figura->getBRDF() == 0){
                    bdrf = phong(figura, p, dirLuz,restaPuntos(origenVista,p));   
                } 
                else if (figura->getBRDF() == 1){
                    bdrf = ward(restaPuntos(origenVista,p), dirLuz, figura->normal(p), p);
                }
                Color auxColor = luz.getColor();
                auxColor.multiplicar(bdrf);
                inicial.sumar(auxColor);
                inicial.sumar(auxC);

                //Caminos especulares
                if ( numeroRebotes > 0){
                    Vector R, dirLuz2 = restaPuntos(p, origenVista);
                    dirLuz2.normalizar();

                    if ( figura->getReflejo() > 0.0){
                        Vector nAux = figura->normal(p);

                        if ( productoEscalar(restaPuntos(origenVista,p), nAux) < 0 ) nAux = valorPorVector(nAux, -1);

                        R = restaVectores(dirLuz2, valorPorVector(valorPorVector(nAux, productoEscalar(dirLuz2,nAux)), 2));
                        R.normalizar();
                        Color auxC = reboteEspecular(figura, p, R, numeroRebotes);
                        auxC.multiplicar(figura->getReflejo());
                        inicial.sumar(auxC);
                    }

                    if( figura->getCoefRefraccion() > 0.0){
                        auxC = refraccionEspecular(figura, p, restaPuntos(p, origenVista), refraccionMedio, figura->getRefraccion(), numeroRebotes);
                        auxC.multiplicar (figura->getCoefRefraccion());
                        inicial.sumar(auxC);
                    }
                }

                inicial.multiplicar(K_LUZ_DIR);
            }

        }
    }
    min = -1;

    if ( indirecto ){
        Figura * choque;
        Punto auxP;
        auxP.set_values(0,0,0);
        Montecarlo montecarlo;

        if ( PATH_TRACING && (seguirCamino < PATH_LEN) ) montecarlo.set_values(1.0,1.0,restaPuntos(p,auxP), figura->normal(p), 1);
        else montecarlo.set_values(1.0,1.0,restaPuntos(p,auxP), figura->normal(p), NUMERO_RAYOS_INDIRECTA);
        
        list<Vector> vecIndir = montecarlo.calcularw();
        if (!PATH_TRACING){
            for ( Vector vec : vecIndir){
                Rayo rayo;
                rayo.set_values(p, vec);
                min = interseccion(rayo, &choque);

                if ( min != -1){
                    Punto puntoRender, origenRayos = camara.getPosicion();
                    Vector direccion = rayo.getVector();
                    puntoRender.set_values(origenRayos.getX() + direccion.getX() * min, origenRayos.getY() + direccion.getY() * min, 
                        origenRayos.getZ() + direccion.getZ() * min);
                    Color cIndir = renderizar(puntoRender, choque, NUMERO_REBOTES, camara.getPosicion(), refraccionMedio, false, 0);
                    cIndir.multiplicar(K_LUZ_INDIR);
                    Vector vAux = restaPuntos(puntoRender,p);
                    vAux.normalizar();

                    if ( figura->getBRDF() == 0){
                        bdrf = phong(figura, p, vAux,restaPuntos(origenVista,p));   
                    } 
                    else if (figura->getBRDF() == 1){
                        bdrf = ward(restaPuntos(origenVista,p), vAux, figura->normal(p), p);
                    }

                    cIndir.multiplicar(bdrf);
                    inicial.sumar(cIndir);

                    min = -1;
                }
            }
        }
        else{
            if ( seguirCamino == 0) return inicial;

            Color cIndir;
            cIndir.set_values(0,0,0, NORMALIZAR_COLORES);
            for (Vector vec : vecIndir){
                Figura * figuraAux;
                Rayo rayo;
                rayo.set_values(p, vec);
                min = interseccion(rayo, &figuraAux);
                if ( min != -1){
                    Punto puntoRender, origenRayos = p;
                    Vector direccion = rayo.getVector();

                    puntoRender.set_values(origenRayos.getX() + direccion.getX() * min, origenRayos.getY() + direccion.getY() * min, 
                        origenRayos.getZ() + direccion.getZ() * min);
                    Color colorAux;

                    if ( !figuraAux->isLuz() ){
                        colorAux = renderizar(puntoRender, figuraAux, NUMERO_REBOTES, camara.getPosicion(), refraccionMedio, true, seguirCamino - 1);
                    }
                    else {
                        Luz lAux = figuraAux->getLuces()[0];
                        lAux.atenuar(min);
                        colorAux = lAux.getColor(); 
                    }
                    Vector vAux = restaPuntos(puntoRender,p);
                    vAux.normalizar();


                    if ( figura->getBRDF() == 0){
                        bdrf = phong(figura, p, vAux,restaPuntos(origenVista,p));   
                    } 
                    else if (figura->getBRDF() == 1){
                        bdrf = ward(restaPuntos(origenVista,p), vAux, figura->normal(p), p);
                    }

                    colorAux.multiplicar(bdrf);
                    colorAux.multiplicar(K_LUZ_INDIR);
                    inicial.sumar(colorAux);
                }
            }
        }
    }

    return inicial;
}

double operadorEscena::phong(Figura * figura, Punto x, Vector luz, Vector vista){
    Vector normal, R;
    double kd = AMBIENTE, La = 0.2, n = 4, ks = 0.3;
    luz.normalizar();
    vista.normalizar();
    normal = figura->normal(x);
    if ( productoEscalar(vista, normal) < 0 ) normal = valorPorVector(normal, -1);

    R = restaVectores(valorPorVector(normal, 2 * productoEscalar(luz, normal)), luz); //Ya normalizado
    double aux = productoEscalar(R, vista);

    return ks * (n + 2)/(2*M_PI) * pow(aux, n);// + (kd/M_PI  * productoEscalar(normal, luz));
}

double operadorEscena::ward(Vector o, Vector i, Vector n, Punto p){
    double ps = 0.75, ax = 0.20, ay = 0.10, especular, exponente;
    Vector h, x, y;
    i.normalizar();
    o.normalizar();
    n.normalizar();
    if ( productoEscalar(i, n) < 0 ) n = valorPorVector(n, -1);

    h = valorPorVector(sumaVectores(i,o), 1.0 / (sumaVectores(i,o).modulo()));
    x = productoVectorial(n, i);
    y = productoVectorial(n, x);
    x.normalizar();
    y.normalizar();
    exponente = -(pow(productoEscalar(h,x)/ax,2) + pow(productoEscalar(h,y)/ay,2)) / pow(productoEscalar(h, n),2);
    especular = ps / (4*M_PI*ax*ay*sqrt(std::abs(productoEscalar(i,n) * productoEscalar(o, n)))) * pow(M_E, exponente);

    if ( especular < 0 ) especular = -especular;
    return especular;
}

Color operadorEscena::reboteEspecular(Figura * figura, Punto origen, Vector R, int numero){
    int min = -1, distancia;
    Color defecto;
    Rayo especular;
    Figura * choque;

    especular.set_values(origen, R);
    defecto.set_values(0,0,0, NORMALIZAR_COLORES);

    min = interseccion(especular, &choque);

    if ( min != -1){
            Punto puntoRender;
            Vector direccion = especular.getVector();
            puntoRender.set_values(origen.getX() + direccion.getX() * min, origen.getY() + direccion.getY() * min, 
                origen.getZ() + direccion.getZ() * min);
            
            return renderizar(puntoRender, choque, numero -1, origen, figura->getRefraccion(), false, 0);
        }
    else{
        return defecto;
    }
}

Color operadorEscena::refraccionEspecular(Figura * figura, Punto origen, Vector vista, double n1, double n2, int numeroRebotes){
    Vector normal = figura->normal(origen), refraccion;
    vista.normalizar();
    normal.normalizar();
    if ( productoEscalar(vista, normal) < 0 ) normal = valorPorVector(normal, -1);
    
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
   
    min = interseccion(rebote, &choque);

    if ( min != -1){
        Punto puntoRender;
   
        puntoRender.set_values(origen.getX() + refraccion.getX() * min, origen.getY() + refraccion.getY() * min,
            origen.getZ() + refraccion.getZ() * min);
   
        return renderizar(puntoRender, choque, numeroRebotes -1, origen, n2, false, 0);
    }
    else{
        return defecto;
    }
}

double operadorEscena::interseccion(Rayo r, Figura ** choque){
    double distancia, min = -1;

    for (int i = 0; i < figuras.size(); i++){
        distancia = figuras[i]->intersectar(r);
        //cout << "Legal " << std::to_string(i) << "\n";

        if ( distancia >= 0 ){
            if (( min == -1) | (distancia < min)){
                min = distancia;
                *choque = figuras[i];
                //cout << "Util\n";
                 if (figuras[i]->isBox()){
                    *choque = ((Box *) (figuras[i]))->store();
                } 
            }
        }
    }
    //cout << "Done\n";

    return min;
}
