/*
   Arduino and MPU6050 Accelerometer and Gyroscope Sensor Tutorial
   by Dejan, https://howtomechatronics.com
   */
#include <lgpio.h>
#include <cmath>
#include <iostream>
#include <unistd.h>
#include <chrono>
using namespace std;
const int MPU = 0x68; // MPU6050 I2C address
float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
float roll, pitch, yaw;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
		auto currentTime = std::chrono::steady_clock::now();
		auto previousTime = std::chrono::steady_clock::now();
int c = 0;

int handle = 0;
void calculate_IMU_error() {
	// We can call this funtion in the setup section to calculate the accelerometer and gyro data error. From here we will get the error values used in the above equations printed on the Serial Monitor.
	// Note that we should place the IMU flat in order to get the proper values, so that we then can the correct values
	// Read accelerometer values 200 times
	while (c < 200) {
		AccX = ((int)lgI2cReadByteData(handle, 0x3B) << 8 | (int)lgI2cReadByteData(handle, 0x3C) ) / 16384.0; // X-axis value
		AccY = ((int) lgI2cReadByteData(handle, 0x3D) << 8 | (int)lgI2cReadByteData(handle, 0x3E))/ 16384.0; // Y-axis value
		AccZ = ((int)lgI2cReadByteData(handle, 0x3F)<< 8 | (int)lgI2cReadByteData(handle, 0x40))/ 16384.0; // Z-axis value
														   // Sum all readings
		AccErrorX = AccErrorX + ((atan((AccY) / sqrt(pow((AccX), 2) + pow((AccZ), 2))) * 180 / M_PI));
		AccErrorY = AccErrorY + ((atan(-1 * (AccX) / sqrt(pow((AccY), 2) + pow((AccZ), 2))) * 180 / M_PI));
		c++;
	}
	//Divide the sum by 200 to get the error value
	AccErrorX = AccErrorX / 200;
	AccErrorY = AccErrorY / 200;
	c = 0;
	// Read gyro values 200 times
	while (c < 200) {
		GyroX = ((int)lgI2cReadByteData(handle, 0x43) << 8 |(int) lgI2cReadByteData(handle, 0x44))/ 131.0; // For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet
		GyroY = ((int)lgI2cReadByteData(handle, 0x45)<< 8 |(int) lgI2cReadByteData(handle, 0x46))/ 131.0;
		GyroZ = ((int)lgI2cReadByteData(handle, 0x47)<< 8 | (int)lgI2cReadByteData(handle, 0x48))/ 131.0;
		// Sum all readings
		GyroErrorX = GyroErrorX + (GyroX / 131.0);
		GyroErrorY = GyroErrorY + (GyroY / 131.0);
		GyroErrorZ = GyroErrorZ + (GyroZ / 131.0);
		c++;
	}
	//Divide the sum by 200 to get the error value
	GyroErrorX = GyroErrorX / 200;
	GyroErrorY = GyroErrorY / 200;
	GyroErrorZ = GyroErrorZ / 200;
}
void setup() {
	handle = lgI2cOpen(1, MPU,0);
	lgI2cWriteByteData(handle, 0x6B, 0x00);

	// Call this function if you need to get the IMU error values for your module
	calculate_IMU_error();
	usleep(20000);
}
int main() {
	setup();
	// === Read acceleromter data === //
	//For a range of +-2g, we need to divide the raw values by 16384, according to the datasheet
	while(1){
		AccX = ((int)lgI2cReadByteData(handle, 0x3B) << 8 | (int)lgI2cReadByteData(handle, 0x3C) ) / 16384.0; // X-axis value
		AccY = ((int) lgI2cReadByteData(handle, 0x3D) << 8 |(int) lgI2cReadByteData(handle, 0x3E))/ 16384.0; // Y-axis value
		AccZ = ((int)lgI2cReadByteData(handle, 0x3F)<< 8 |(int) lgI2cReadByteData(handle, 0x40))/ 16384.0; // Z-axis value
														   // Calculating Roll and Pitch from the accelerometer data
		accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / M_PI) - 0.58; // AccErrorX ~(0.58) See the calculate_IMU_error()custom function for more details
		accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / M_PI) + 1.58; // AccErrorY ~(-1.58)
													// === Read gyroscope data === //
		previousTime = currentTime;        // Previous time is stored before the actual time read
		currentTime = std::chrono::steady_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - previousTime).count() / 1000.0;
								   
		GyroX = (lgI2cReadByteData(handle, 0x43) << 8 | lgI2cReadByteData(handle, 0x44))/ 131.0; // For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet
		GyroY = (lgI2cReadByteData(handle, 0x45)<< 8 | lgI2cReadByteData(handle, 0x46))/ 131.0;
		GyroZ = (lgI2cReadByteData(handle, 0x47)<< 8 | lgI2cReadByteData(handle, 0x48))/ 131.0;
		// Correct the outputs with the calculated error values
		GyroX = GyroX + 0.56; // GyroErrorX ~(-0.56)
		GyroY = GyroY - 2; // GyroErrorY ~(2)
		GyroZ = GyroZ + 0.79; // GyroErrorZ ~ (-0.8)
				      // Currently the raw values are in degrees per seconds, deg/s, so we need to multiply by sendonds (s) to get the angle in degrees
		gyroAngleX = gyroAngleX + GyroX * elapsedTime; // deg/s * s = deg
		gyroAngleY = gyroAngleY + GyroY * elapsedTime;
		yaw =  yaw + GyroZ * elapsedTime;
		// Complementary filter - combine acceleromter and gyro angle values
		roll = 0.96 * gyroAngleX + 0.04 * accAngleX;
		pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;
		cout << "Roll: " << roll << "\t Pitch: " << pitch << '\r';
	}
}

