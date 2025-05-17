#pragma once
#include "pca9685.h"

class ESC {
	public:
		// Base Constructor
		ESC();
		
		// Constructor to set pin
		ESC(int pin);

		// Thruster with pin and rest
		ESC(int pin, int rest);

		// Thruster with pin and rest
		ESC(int pin, int rest, int offset);

		// Set PWM
		void setPWM(int val, pca9685 &driver);

		// Set Power Based on power and offset
		void setPower(double val, pca9685 &driver);
		
		// Get PWM value and Power
		int getPWM() const;	
		float getPower() const;	

	private:
		int pin;
		int rest = 1500;
		int offset = 400;
		int pwm; 
		float power;
		
};
