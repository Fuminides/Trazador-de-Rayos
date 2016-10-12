#ifndef COLOR_H_
#define COLOR_H_ 

#include <string>

class Color {
	int red, green, blue;

	public:
		void set_values(int,int,int);
		std::string splash();
};

#endif