#include <iostream>
#include <SDL2/SDL.h>
#include <lgpio.h>
#include <thread>
#include "controller.h"
using namespace std;

const int THRUSTER1 = 26;
const int THRUSTER2 = 6;
const int THRUSTER3 = 5;
const int THRUSTER4 = 0;

const int LFLAGS = 0;

double power = 0.5;
bool thing = true;

void controls(SDL_Joystick* controller){
	SDL_Event event;
	while (1){
		while (SDL_PollEvent(&event)){
			if (event.type == SDL_QUIT) thing = false;
		}
		// Get x axis
		Sint16 axisX = SDL_JoystickGetAxis(controller, 0);

		// Get y Axis
		Sint16 axisY = SDL_JoystickGetAxis(controller, 1);

		// Map to -1.0 to 1.0
		power = axisY / 32768.0;
	}

}

int main(){
	int h;
	h = lgGpiochipOpen(0); // Initialize GPIO
	//SDL_Init(SDL_INIT_JOYSTICK); // Initialize SDL

	// Check if Joystick is connected
//	if (SDL_NumJoysticks() < 1) {
//		std::cerr << "No joysticks connected!" << std::endl;
//		SDL_Quit();
//		return -1;
//	}

	//SDL_Joystick* controller = SDL_JoystickOpen(0); // Get the controller

	//thread control(controls, controller); // Start controller thread

	if (lgGpioClaimOutput(h, LFLAGS, THRUSTER1, 0) == LG_OKAY)
	{
		cout << "initial signal" << endl;
		// Inital PWM output
		for (int i = 0; i < 100000; i++){
			lgTxServo(h, THRUSTER3, 1500, 50, 0, 0); /* 1500 microseconds, 50 Hz */
			lgTxServo(h, THRUSTER1, 1500, 50, 0, 0); /* 1500 microseconds, 50 Hz */
			lgTxServo(h, THRUSTER2, 1500, 50, 0, 0); /* 1500 microseconds, 50 Hz */
			lgTxServo(h, THRUSTER4, 1500, 50, 0, 0); /* 1500 microseconds, 50 Hz */
		}

		usleep(1000000);


		// Send controller signal to thruster
		while(thing){
			lgTxServo(h, THRUSTER3, 1500 + power * 400, 50, 0, 0); /* 1500 microseconds, 50 Hz */
			lgTxServo(h, THRUSTER1, 1500 + power * 400, 50, 0, 0); /* 1500 microseconds, 50 Hz */
			lgTxServo(h, THRUSTER2, 1500 + power * 400, 50, 0, 0); /* 1500 microseconds, 50 Hz */
			lgTxServo(h, THRUSTER4, 1500 + power * 400, 50, 0, 0); /* 1500 microseconds, 50 Hz */
		}
	} else cout << "bruh" << endl;

	// Join Thread
//	control.join();
//
//	SDL_JoystickClose(controller);
//	SDL_Quit();

	lgGpiochipClose(h);
}
