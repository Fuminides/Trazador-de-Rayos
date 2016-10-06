#ifndef objeto_cartesiano_h_
#define objeto_cartesiano_h_

class ObjetoCartesiano {
 	protected:
	 	float valores[4];

	public:
	 	void set_values(float, float, float);
	 	void set_values(float, float, float, float);

	 	void set_X(float);
	 	void set_Y(float);
	 	void set_Z(float);

	 	float getX();
	 	float getY();
	 	float getZ();
	 	float getD();

	 	float get(int indice);

	 	virtual bool esVector() = 0; 
 };

#endif