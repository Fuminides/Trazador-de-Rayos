#ifndef COLOR_H_
#define COLOR_H_ 

#include <string>

class Color {
	unsigned char red, green, blue;

	public:
		void set_values(int,int,int);
		unsigned char splashR();
		unsigned char splashG();
		unsigned char splashB();
};

#endif