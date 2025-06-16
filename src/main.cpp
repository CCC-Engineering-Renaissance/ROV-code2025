#include <iostream>
#include <lgpio.h>
#include <thread>
#include <cmath>
#include <vector>
#include <string>
#include <Eigen/Dense>
#include <Eigen/QR>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <iomanip>
#include <chrono>

#include "pca9685.h"
#include "mpuXX50.h"
#include "connection.h"
#include "matrix.h"
#include "thruster.h"

using namespace std;
int sampleRate = 4000;
int time2Delay;
int fd;
float dt;

MPUXX50 *mpuXX50;
// I2C read and write functions
int i2c_read(int addr, unsigned char *data, char len){
	if (read(addr, data, len) != len){
		return -1;
	} else {
		return 0;
	}
};

int i2c_write(int addr, unsigned char *data, char len){
	if (write(addr, data, len) != len){
		return -1;
	} else {
    return 0;
  }
};

// Time stabilization function
float timeSync(auto t1){
	// Find duration to sleep thread
	auto t2 = std::chrono::high_resolution_clock::now();
	time2Delay = std::chrono::duration<float, std::micro>(t2-t1).count();

	// Sleep thread
	std::this_thread::sleep_for(std::chrono::microseconds(sampleRate-time2Delay));

	// Calculate dt
	auto t3 = std::chrono::high_resolution_clock::now();
	dt = (std::chrono::duration<float, std::micro>(t3-t1).count()) * 1E-6;

	// Return dt and begin main loop again
	return dt;
}

// Open the I2C Bus
char *filename = (char*)"/dev/i2c-1";

// Setup the MPU class
struct i2c_device_t i2c_dev;

const int LFLAGS = 0;
double power = 0.5;
bool thing = true;
int h = lgGpiochipOpen(0); 

pca9685 driver;

vector<bool> thrusts(13,true) ;
//vector<int> things = {0, 1, 2, 3, 4, 5, 6, 7};
vector<int> things = {1, 2, 0, 4, 3, 7, 6, 5};
vector<ESC> escs;

uint16_t  lerp(uint16_t a, uint16_t b, float t){
	return a + t * (b - a);
}


void loop(){
	while (1){
		// Get how much power to set each thruster 
		double error = (state.angle - mpuXX50->attitude.pitch )/ 130.0;
		double pitch = 0.0;
		cout << state.als << " ";
		if (state.als)
			pitch = error;
		else
			pitch = state.pitch;

		Eigen::MatrixXf vals = calcs(state.forward * 2, state.strafe * 2, state.vertical * 2, state.roll * 2.5, pitch * 2.5, state.yaw * 2.5);

		// Set each Thruster's PWM value
		for (int i = 0; i < 8; i++){
			if (thrusts.at(i)) {
				// Calculate PWM value from power

				float test = 1500 + vals(i) * 450;
				
				if (vals(i) > 1.0)
				{
					escs.at(i).setPower(1.0, driver);
				}
				else if (vals(i) < -1.0)
				{
					escs.at(i).setPower(-1.0, driver);
				}
				else
				{
					escs.at(i).setPower(vals(i), driver);
					//cout << i << ": " << test << "  ";
				}
			}
			else {
				cout << i << ": DEACTIVATED" << " ";
				escs.at(i).setPower(0, driver);
			}
		}

		// Set Each Claw
		if (thrusts.at(8)) {
			float test = 1440 + state.clawRotate * 200;
			//cout << "Rotate" << ": " << state.clawRotate << "  ";
			driver.setPWM(8, test);
		}
		else {
			cout << state.clawRotate << ": DEACTIVATED" << " ";
			driver.setPWM(8, 0);
		}

		if (thrusts.at(9)) {
			float test = 1440 + state.clawOpen * 400;
			//cout << "Open" << ": " << state.clawOpen << "  ";
			driver.setPWM(9, test);
		}
		else {
			cout << state.clawOpen << ": DEACTIVATED" << " ";
			driver.setPWM(9, 0);
		}

		if (thrusts.at(10)) {
			float test = 1440 + state.clawPitch * 400;
			//cout << "clawPitch" << ": " << state.clawPitch << "  ";
			driver.setPWM(10, test);
		}
		else {
			cout << state.clawPitch << ": DEACTIVATED" << " ";
			driver.setPWM(10, 0);
		}
		if (thrusts.at(11)) {
			float test = 1460 + state.claw1Open * 400;
			//cout << "Open" << ": " << state.claw1Open << "  ";
			driver.setPWM(11, test);
		}
		else {
			cout << state.claw1Open << ": DEACTIVATED" << " ";
			driver.setPWM(11, 0);
		}
	
		cout << '\r';
		cout.flush();
	}
}

#define PCA9685_ADDR 0x40

int main(){
	// Start I2C
	if ((fd = open(filename, O_RDWR)) < 0){
    std::cout << "Failed to open the i2c bus" << std::endl;
    return -1;
	}

  // Connect on 0x68
	if (ioctl(fd, I2C_SLAVE, 0x68) < 0){
    std::cout << "Failed to acquire bus access and/or talk to slave." << std::endl;
		return -1;
	}

  // Prepare I2C functions for read and write
  i2c_dev.i2c_write = (i2c_read_write_t) &i2c_write;
  i2c_dev.i2c_read = (i2c_read_write_t) &i2c_read;

  // Connect to sensor using file identifier
  mpuXX50 = new MPUXX50(fd, i2c_dev);

  // Initialize the IMU and set the senstivity values
  std::cout << "IMU initialize. Pass/Fail: ";
  std::cout << mpuXX50->initIMU(MPU9250) << std::endl;
  mpuXX50->getAres(AFS_4G);
  mpuXX50->getGres(GFS_500DPS);
  sleep(1);

  // Calibrate the gyroscope
  gyro_cal_t gyro_cal;
  std::cout << "Calibrating gyroscope, hold IMU stationary." << std::endl;
  sleep(2);
  mpuXX50->gyroCalibration(1000);

	// Set PWM Freq of the driver
	driver.setFrequency(50);

	// Start server thread
	thread control(server);
	
	for (int i = 0; i < 8; i++){
		ESC esc(things.at(i), 1460, 250);
		escs.push_back(esc);
	}	

	// Send a rest signal of 1460 Hz to prime thrusters
	// idk why its not 1500
	for (int i = 0; i < 8; i++){
		escs.at(i).setPWM(1460, driver);
	}
	usleep(10000);

	// Prime Claw, donald said we cant use a for loop for some reason
	// The drone motors have to go to the lower limit, upper limit, then rest width to prime.
	driver.setPWM(8, 1000);
	driver.setPWM(8, 2000);
	driver.setPWM(8, 1460);
	
	driver.setPWM(9, 1000);
	driver.setPWM(9, 2000);
	driver.setPWM(9, 1460);
	driver.setPWM(10, 1000);
	driver.setPWM(10, 2000);
	driver.setPWM(10, 1460);
	driver.setPWM(11, 1000);
	driver.setPWM(11, 2000);
	driver.setPWM(11, 1460);
	
	usleep(10000);

	// Start thread to send data to thrusters
	thread thrusters(loop);

	// Allows us to turn off and on thrusters
	int num;

  // Run this forever
  while(true) {
		auto loopTimer = std::chrono::high_resolution_clock::now();
    mpuXX50->compFilter(dt, 0.95);


		// Stabilize the data rate
		dt = timeSync(loopTimer);
  }

	while (1){
		cin >> num;
		// Disable thrsuster
		thrusts.at(num) = !thrusts.at(num);
	}

	// Join Threads
	control.join();
	thrusters.join();

	lgGpiochipClose(h);
}
