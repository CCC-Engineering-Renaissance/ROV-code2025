#pragma once
#include <lgpio.h>

class pca9685{
	public:
		pca9685();
		pca9685(unsigned char address);
		void setFrequency(int freq);
		void setPWM(int pin, uint16_t signal);
	private:
		unsigned char address;
		int handle;

};


