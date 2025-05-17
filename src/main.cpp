#include <iostream>
#include <lgpio.h>
#include <thread>
#include <cmath>
#include <vector>
#include <string>
#include <Eigen/Dense>
#include <Eigen/QR>
#include <mqtt/async_client.h>

#include "pca9685.h"
#include "connection.h"
#include "matrix.h"
#include "thruster.h"

using namespace std;
const int LFLAGS = 0;
double power = 0.5;
bool thing = true;
int h = lgGpiochipOpen(0); 

pca9685 driver;

vector<bool> thrusts(10,false) ;
vector<int> things = {4, 1, 7, 0, 3, 6, 5, 2};
vector<ESC> escs;

uint16_t  lerp(uint16_t a, uint16_t b, float t){
	return a + t * (b - a);
}

void loop(){
	while (1){
		// Get how much power to set each thruster 
		Eigen::MatrixXf vals = calcs(state.forward * 2, state.strafe * 2, state.vertical * 2, state.roll * 2.5, state.pitch * 2.5, state.yaw * 2.5);

		// Set each Thruster's PWM value
		for (int i = 0; i < 8; i++){
			if (thrusts.at(i)) {
				// Calculate PWM value from power
				float test = 1460 + vals(i) * 250;
				cout << i << ": " << test << "  ";
				
				escs.at(i).setPower(vals(i), driver);
			}
			else {
				cout << i << ": DEACTIVATED" << " ";
				escs.at(i).setPower(1460, driver);
			}
		}

		// Set Each Claw
		if (thrusts.at(8)) {
			float test = 1440 + state.clawRotate * 400;
			cout << "Rotate" << ": " << state.clawRotate << "  ";
			driver.setPWM(8, test);
		}
		else {
			cout << state.clawRotate << ": DEACTIVATED" << " ";
			driver.setPWM(8, 0);
		}

		if (thrusts.at(9)) {
			float test = 1440 + state.clawOpen * 400;
			cout << "Open" << ": " << state.clawOpen << "  ";
			driver.setPWM(9, test);
		}
		else {
			cout << state.clawOpen << ": DEACTIVATED" << " ";
			driver.setPWM(9, 0);
		}
		cout << '\r';
		cout.flush();

	}
}

#define PCA9685_ADDR 0x40

int main(){
	// Set PWM Freq of the driver
	driver.setFrequency(50);

	// Start server thread
	thread control(server);
	
	for (int i = 0; i < 8; i++){
		ESC esc(things.at(i), 1460, 250);
		escs.push_back(esc);
	}	

	// Prime Thrusters 
	for (int i = 0; i < 8; i++){
		escs.at(i).setPWM(1460, driver);
	}
	usleep(10000);

	// Prime Claw, donald said we cant use a for loop for some reason
	driver.setPWM(8, 1000);
	driver.setPWM(8, 2000);
	driver.setPWM(8, 1460);
	driver.setPWM(9, 1000);
	driver.setPWM(9, 2000);
	driver.setPWM(9, 1460);
	usleep(10000);

	// Start thread to send data to thrusters
	thread thrusters(loop);

	// Allows us to turn off and on thrusters
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
