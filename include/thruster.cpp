#include "thruster.h"

// CONSTRUCTORS
ESC::ESC(){
	pin = -1;
}

ESC::ESC(int pin){
	if (pin < 0 || pin > 15)
		pin = -1;

	this->pin = pin;
}

ESC::ESC(int pin, int rest){
	if (pin < 0 || pin > 15)
		pin = -1;

	this->rest = rest;
	this->pin = pin;
}

ESC::ESC(int pin, int rest, int offset){
	if (pin < 0 || pin > 15)
		pin = -1;

	this->rest = rest;
	this->pin = pin;
	this->offset = offset;
}

void ESC::setPWM(int val, pca9685 &driver){

	if (val < this->rest - this->offset) val = this->rest - this->offset;
	else if (val > this->rest + this->offset) val = this->rest + this->offset;
	this->pwm = val;

	driver.setPWM(this->pin, this->pwm);
}

void ESC::setPower(double val, pca9685 &driver){
	// Limit Vals to -1 to 1
	if (val < -1) val = -1.0;
	if (val > 1) val = 1.0;
	
	this->pwm = this->rest + this->offset * val;
	this->power = power;

	driver.setPWM(this->pin, this->pwm);
}

int ESC::getPWM() const {
	return this->pwm;
}

float ESC::getPower() const {
	return this->power;
}

