#include "pca9685.h"
#include <unistd.h>
#include <cmath>

pca9685::pca9685(){
	address = 0x40;
	handle = lgI2cOpen(1, address, 0);
	lgI2cWriteByteData(handle, 0x00, 0x00);
}
pca9685::pca9685(unsigned char address){
	this->address = address;
	handle = lgI2cOpen(1, address, 0);
}
void pca9685::setFrequency(int freq){
	unsigned char test = round(25000000/(4096 * freq)) - 1;
	unsigned char thing = lgI2cReadByteData(handle, 0x00);
	lgI2cWriteByteData(handle, 0x00, thing | 0x10);
	lgI2cWriteByteData(handle, 0xFE, 0x50);
	lgI2cWriteByteData(handle, 0x00, thing);
	usleep(5000);
	lgI2cWriteByteData(handle, 0x00, 0x01);
}
void pca9685::setPWM(int pin, uint16_t signal){
	uint16_t freq = (482.0 / 1500.0) * signal;
	//uint16_t freq = signal;

	lgI2cWriteByteData(handle, 0x06 + 4 * pin, 0);
	lgI2cWriteByteData(handle, 0x06 + 4 * pin + 1, 0);
	lgI2cWriteByteData(handle, 0x06 + 4 * pin + 2, freq & 0xFF);
	lgI2cWriteByteData(handle, 0x06 + 4 * pin + 3, freq >> 8);
}
