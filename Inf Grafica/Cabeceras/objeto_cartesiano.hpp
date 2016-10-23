#ifndef objeto_cartesiano_h_
#define objeto_cartesiano_h_

#include <string>

class ObjetoCartesiano {
 	protected:
	 	double valores[4];

	public:
	 	void set_values(double, double, double);
	 	void set_values(double, double, double, double);

	 	void set_X(double);
	 	void set_Y(double);
	 	void set_Z(double);

	 	double getX();
	 	double getY();
	 	double getZ();
	 	double getD();

	 	double get(int indice);

	 	virtual bool esVector() = 0; 

	 	std::string to_string();
 };

#endif