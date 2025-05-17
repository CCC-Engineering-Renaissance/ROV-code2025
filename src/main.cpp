#include <iostream>
#include <SDL2/SDL.h>
#include <lgpio.h>
#include <thread>
#include <cmath>
#include <vector>
#include <string>
#include <Eigen/Dense>
#include <Eigen/QR>
#include <mqtt/async_client.h>

#include "pca9685.h"
//#include "xbox.h"
//#include "mqtt.h"
#include "connection.h"
#include "matrix.h"
using namespace std;


const int THRUSTER1 = 19;
const int THRUSTER2 = 26;
const int THRUSTER3 = 13;
const int THRUSTER4 = 0;

const int LFLAGS = 0;

double power = 0.5;
bool thing = true;

int h = lgGpiochipOpen(0); 
pca9685 driver;
vector<bool> thrusts(10, true);
//vector<int> things = {3, 2, 7, 4, 0, 5, 6, 1};
//vector<int> things = {3, 2, 7, 4, 0, 5, 6, 1};
vector<int> things = {4, 1, 7, 0, 3, 6, 5, 2};

uint16_t  lerp(uint16_t a, uint16_t b, float t){
	return a + t * (b - a);
}

void loop(){
	uint16_t test = 1500;
	uint16_t back = test;
	while (1){
		double smoothing = 0.8;

		//Eigen::MatrixXf vals = calcs(state.forward * 2, state.strafe * 2, state.vertical * 2, state.roll * 1.25, state.pitch * 1.25, state.yaw * 1.25);
		Eigen::MatrixXf vals = calcs(state.forward * 2, state.strafe * 2, state.vertical * 2, state.roll * 2.5, state.pitch * 2.5, state.yaw * 2.5);
		//Eigen::MatrixXf vals = calcs(state.forward * 1.1, state.strafe * 1.1, state.vertical * 1.1, state.roll * 1.1, state.pitch * 1.1, state.yaw * 1.1);
		//cout << vals;


		for (int i = 0; i < 8; i++){
			if (thrusts.at(i)) {
				test = 1460 + vals(i) * 250;
				if (test >= 1650) test = 1650;
				if (test <= 1350) test = 1350;
				//test = vals(i);
				//cout << i << ": " << lerp(back, test, smoothing) << "  ";
				//driver.setPWM(i, lerp(back, test, smoothing));
				//publish(string("thrusters/thruster") + to_string(i), to_string(vals(i)));
				cout << i << ": " << test << "  ";
				//cout << vals(i) << " ";
				driver.setPWM(things.at(i), test);
			}
			else {
				cout << i << ": DEACTIVATED" << " ";
				driver.setPWM(i, 1460);
			}
		}
			if (thrusts.at(8)) {
				test = 1440 + state.clawRotate * 400;
				//driver.setPWM(i, lerp(back, test, smoothing));
				//publish(string("thrusters/thruster") + to_string(8), to_string(test));
				cout << "Rotate" << ": " << state.clawRotate << "  ";
				//cout << vals(i) << " ";
				driver.setPWM(8, test);
			}
			else {
				cout << state.clawRotate << ": DEACTIVATED" << " ";
				driver.setPWM(8, 0);
			}

			if (thrusts.at(9)) {
				test = 1440 + state.clawOpen * 400;
				//driver.setPWM(i, lerp(back, test, smoothing));
				//publish(string("thrusters/thruster") + to_string(9), to_string(test));
				cout << "Open" << ": " << state.clawOpen << "  ";
				//cout << vals(i) << " ";
				driver.setPWM(9, test);
			}
			else {
				cout << state.clawOpen << ": DEACTIVATED" << " ";
				driver.setPWM(9, 0);
			}

		cout << '\r';
		cout.flush();
		back = lerp(back, test, smoothing);
	}
}

#define PCA9685_ADDR 0x40

int main(){
	driver.setFrequency(50);

	// Start thread to read controller input
	thread control(server);

	// Prime Thrusters and Claw
	for (int i = 0; i < 8; i++){
		driver.setPWM(i, 1460);
	}
	for (int i = 8; i < 10; i++){
		driver.setPWM(i, 0);
	}
	usleep(10000);
	driver.setPWM(8, 1000);
	driver.setPWM(8, 2000);
	driver.setPWM(8, 1460);
	driver.setPWM(9, 1000);
	driver.setPWM(9, 2000);
	driver.setPWM(9, 1460);
	usleep(10000);

	// Start thread to send data to thrusters
	thread thrusters(loop);

	int num;
	while (1){
		cin >> num;
		thrusts.at(num) = !thrusts.at(num);
	}

	// Join Threads
	control.join();
	thrusters.join();

	lgGpiochipClose(h);
}
