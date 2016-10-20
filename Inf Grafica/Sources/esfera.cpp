#include "esfera.hpp"

    /*
     * Devuelve el radio de la esfera
     */ 
    float Esfera::getRadio(){
        return radio;
    }
    /*
     * Asigna un valor al radio 
     */
    void Esfera::setRadio(float r){
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
    void Esfera::setOrigen(float x, float y, float z){
            origen.set_values(x,y,z);
            color.set_values(0,0,0);
    }
    /*
     * Devuelve la superficie de la esfera
     */
    float Esfera::superficie(){
        return radio * radio * M_PI;
    }

    /*
     * Devuelve la distancia a la que un Rayo r intercepta a una esfera f, -1 en caso de que no 
     * la intercepte
     */
    float Esfera::intersectar(Rayo r){
        Punto O= r.getOrigen();
        Vector D = r.getVector();

        Punto C= getOrigen();

        float OC= puntoPorPunto(O, C);
        float OO= puntoPorPunto(O, O);
        float CC= puntoPorPunto(C, C);
        float OD = puntoPorVector(O, D);
        float CD = puntoPorVector(C, D);

        float raiz=(2*OD - 2*CD) * (2*OD - 2*CD) - 4*OO*(OO+CC-2*OC-(getRadio()*getRadio()));

        //std::cout << "CD: " << std::to_string(CD) << ", OD: " << std::to_string(OD) << '\n';
        
        if (raiz<0){
            //std::cout << "CD: " << std::to_string(CD) << ", OD: " << std::to_string(OD) << '\n';
            std::cout << "Raiz: " << std::to_string(raiz) << '\n';
            return -1;
        }
        else{
            raiz=sqrt(raiz);
            float p1=-(2*puntoPorVector(O, D) -2* puntoPorVector(C, D));
            float d1=(p1+raiz)/(2*OO);
            float d2=(p1-raiz)/(2*OO);
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
        }
        
}

float Esfera::puntoPorPunto(Punto p1, Punto p2){
    return p1.getX() * p2.getX() + p1.getY() * p2.getY() + p1.getZ() * p2.getZ();
}

float Esfera::puntoPorVector(Punto p1, Vector p2){
    return p1.getX() * p2.getX() + p1.getY() * p2.getY() + p1.getZ() * p2.getZ(); 
}

Vector Esfera::normal(Punto p){
    Vector v;
    v = restaPuntos(p, origen);
    v.normalizar();
    return v;
}