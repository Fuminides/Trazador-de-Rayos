#ifndef objeto_cartesiano_h_
#define objeto_cartesiano_h_

class ObjetoCartesiano {
 	protected:
	 	int x, y, z;

	public:
	 	void set_values(int, int, int);

	 	void set_X(int);
	 	void set_Y(int);
	 	void set_Z(int);

	 	int getX();
	 	int getY();
	 	int getZ();

	 	virtual bool esVector() = 0; 
 };

#endif