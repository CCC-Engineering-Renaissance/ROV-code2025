#include <iostream>
#include <SDL2/SDL.h>
#include <lgpio.h>
#include <thread>
using namespace std;

const int THRUSTER1 = 26;
const int LFLAGS = 0;

double power = 0.0;

void controls(SDL_Joystick* controller){
	SDL_Event event;
	while (1){
		while (SDL_PollEvent(&event)){
			if (event.type == SDL_QUIT) break;
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
	SDL_Init(SDL_INIT_JOYSTICK); // Initialize SDL

	// Check if Joystick is connected
	if (SDL_NumJoysticks() < 1) {
		std::cerr << "No joysticks connected!" << std::endl;
		SDL_Quit();
		return -1;
	}

	SDL_Joystick* controller = SDL_JoystickOpen(0); // Get the controller

	thread control(controls, controller); // Start controller thread

	if (lgGpioClaimOutput(h, LFLAGS, THRUSTER1, 0) == LG_OKAY)
	{
		// Inital PWM output
		for (int i = 0; i < 500; i++){
			lgTxServo(h, THRUSTER1, 1500, 50, 0, 0); /* 1500 microseconds, 50 Hz */
		}

		// Send controller signal to thruster
		while(1){
			lgTxServo(h, THRUSTER1, 1500 + power * 400, 50, 0, 0); /* 1500 microseconds, 50 Hz */
		}
	}

	// Join Thread
	control.join();

	SDL_Quit();
	SDL_JoystickClose(controller);
	lgGpiochipClose(h);
}
