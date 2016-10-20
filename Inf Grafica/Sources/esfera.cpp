#include "esfera.hpp"

    /*
     * Devuelve el radio de la esfera
     */ 
    double Esfera::getRadio(){
        return radio;
    }
    /*
     * Asigna un valor al radio 
     */
    void Esfera::setRadio(double r){
        radio=r;
    }
    /*
     * Devuelve el origen de la esfera en el plano
     */
    Punto Esfera::getOrigen(){
        return origen;
    }
    /*
     * Asigna el valor del origen de la esfera
     */
    void Esfera::setOrigen(double x, double y, double z){
            origen.set_values(x,y,z);
            color.set_values(0,0,0);
    }
    /*
     * Devuelve la superficie de la esfera
     */
    double Esfera::superficie(){
        return radio * radio * M_PI;
    }

    /*
     * Devuelve la distancia a la que un Rayo r intercepta a una esfera f, -1 en caso de que no 
     * la intercepte
     */
    double Esfera::intersectar(Rayo r){
        Punto O= r.getOrigen();
        Vector D = r.getVector();
        Punto C= getOrigen();

        double b = puntoPorVector(O, D);
        b -= puntoPorVector(C, D);
        b = b*2;

        double c = puntoPorPunto(O, O);
        c += puntoPorPunto(C, C);
        c -= 2 * puntoPorPunto(C, O);
        c -= radio * radio;

        double aux = b * b - 4 * c;

        if ( aux < 0.0 ){
            return -1;
        } 

        double t1 = 0.0 - b + sqrt(aux);
        t1 = t1 / 2.0;
        double t2 = 0.0 - b - sqrt(aux);
        t2 = t2 / 2.0;

        if (t1 == t2 && t1>0.0){
            return t1;
        }
        else if (t1>=0.0 && t2<0.0){
            return t1;
        }
        else if( t1>=0.0 && t2>0.0){
            return t2;
        }
        else{
            return -1.0;
        }
        
        /*
        double OC= puntoPorPunto(O, C);
        double OO= puntoPorPunto(O, O);
        double CC= puntoPorPunto(C, C);
        double OD = puntoPorVector(O, D);
        double CD = puntoPorVector(C, D);

        double raiz=(2*OD - 2*CD) * (2*OD - 2*CD) - 4*OO*(OO+CC-2*OC-(getRadio()*getRadio()));

        //std::cout << "CD: " << std::to_string(CD) << ", OD: " << std::to_string(OD) << '\n';
        
        if (raiz<0){
            //std::cout << "CD: " << std::to_string(CD) << ", OD: " << std::to_string(OD) << '\n';
            std::cout << "Raiz: " << std::to_string(raiz) << '\n';
            return -1;
        }
        else{
            raiz=sqrt(raiz);
            double p1=-(2*puntoPorVector(O, D) -2* puntoPorVector(C, D));
            double d1=(p1+raiz)/(2*OO);
            double d2=(p1-raiz)/(2*OO);
            std::cout << "Raiz, Distancias: " << std::to_string(raiz) << ", " << std::to_string(d1) << " y " << std::to_string(d2) << '\n';
            if (d1 >= 0 && d1<=d2){
               return d1;
            }
            else if(d2 >= 0 && d2<d1){
                return d2; 
            }

            else{
                return -1;
            }
        }*/
        
}

double Esfera::puntoPorPunto(Punto p1, Punto p2){
    return p1.getX() * p2.getX() + p1.getY() * p2.getY() + p1.getZ() * p2.getZ();
}

double Esfera::puntoPorVector(Punto p1, Vector p2){
    return p1.getX() * p2.getX() + p1.getY() * p2.getY() + p1.getZ() * p2.getZ(); 
}

Vector Esfera::normal(Punto p){
    Vector v;
    v = restaPuntos(p, origen);
    v.normalizar();
    return v;
}