#ifndef objeto_cartesiano_h_
#define objeto_cartesiano_h_

class ObjetoCartesiano {
 	protected:
	 	float x, y, z;

	public:
	 	void set_values(float, float, float);

	 	void set_X(float);
	 	void set_Y(float);
	 	void set_Z(float);

	 	float getX();
	 	float getY();
	 	float getZ();

	 	virtual bool esVector() = 0; 
 };

#endif