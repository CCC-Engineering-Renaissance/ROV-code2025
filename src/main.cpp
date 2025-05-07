#include <iostream>
#include <SDL2/SDL.h>
#include <lgpio.h>
#include <thread>
#include <cmath>
#include <vector>
#include <string>
#include <Eigen/Dense>
#include <Eigen/QR>

#include "pca9685.h"
#include "xbox.h"
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

uint16_t  lerp(uint16_t a, uint16_t b, float t){
	return a + t * (b - a);
}

void loop(){
	uint16_t test = 1500;
	uint16_t back = test;
	while (isRunning()){
		double smoothing = 0.8;

		//Eigen::MatrixXf vals = calcs(state.forward, state.strafe, state.vertical, state.roll, state.pitch, state.yaw);


		for (int i = 0; i < 8; i++){
			if (thrusts.at(i)) {
				test = 1500 + state.forward * 200;
				cout << i << ": " << lerp(back, test, smoothing) << "  ";
				driver.setPWM(i, lerp(back, test, smoothing));
				//cout << i << ": " << test << "  ";
				//driver.setPWM(i, test);
			}
			else {
				cout << i << ": DEACTIVATED" << " ";
				driver.setPWM(i, 1500);
			}
		}
		cout << '\r';
		cout.flush();
		back = lerp(back, test, smoothing);
	}
}

#define PCA9685_ADDR 0x40

int main(){
	SDL_Init(SDL_INIT_GAMECONTROLLER); // Initialize SDL

	SDL_GameController* controller = SDL_GameControllerOpen(0); // Get the controller
	if (!controller) {
		cout << "No controller detected" << endl;
		exit(1);
	}

	driver.setFrequency(50);

	// Start thread to read controller input
	thread control(controls1, controller);

	for (int i = 0; i < 10; i++){
		driver.setPWM(i, 1500);
	}
	usleep(10000);

	// Start thread to send data to thrusters
	thread thrusters(loop);

	int num;
	while (isRunning()){
		cin >> num;
		thrusts.at(num) = !thrusts.at(num);
	}

	// Join Threads
	control.join();
	thrusters.join();

	SDL_GameControllerClose(controller);
	SDL_Quit();

	lgGpiochipClose(h);
}
