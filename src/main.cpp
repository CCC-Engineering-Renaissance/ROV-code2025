#include <iostream>
#include <SDL2/SDL.h>
#include <lgpio.h>
#include <thread>
#include <cmath>

#include "pca9685.h"
#include "xbox.h"
using namespace std;


const int THRUSTER1 = 19;
const int THRUSTER2 = 26;
const int THRUSTER3 = 13;
const int THRUSTER4 = 0;

const int LFLAGS = 0;

double power = 0.5;
bool thing = true;

int h;
uint16_t  lerp(uint16_t a, uint16_t b, float t){
	return a + t * (b - a);
}
class Thruster{
	int pin;
	thread t;

	public:
	Thruster(){}
	Thruster(int newPin){pin = newPin; t = thread(this->loop, pin);}

	static void loop(int pin){
		cout << "Bruh" << endl;
		for (int i = 0; i < 100000; i++){
			lgTxServo(h, pin, 1500, 50, 0, 0); /* 1500 microseconds, 50 Hz */
		}

		usleep(10000);

		// Send controller signal to thruster
		while(thing){
			lgTxServo(h, pin, 1500 + power * 400, 50, 0, 0); /* 1500 microseconds, 50 Hz */
		}	
	}

};

#define PCA9685_ADDR 0x40

int main(){
	h = lgGpiochipOpen(0); 
	SDL_Init(SDL_INIT_GAMECONTROLLER); // Initialize SDL

	SDL_GameController* controller = SDL_GameControllerOpen(0); // Get the controller

	pca9685 driver;
	driver.setFrequency(75);

	thread control(controls1, controller);
	uint16_t test = 1500;

	for (int i = 0; i < 10; i++){
		driver.setPWM(i, 1500);
	}

	usleep(10000);


	uint16_t back = test;


	while (isRunning()){
		test = 1500 + 400 * state.forward;
		double smoothing = 0.8;
		cout << lerp(back, test, smoothing) << '\r';
		cout.flush();
			lgTxServo(h, 14, 1000, 50, 0, 0);

			lgTxServo(h, 14, 2000, 50, 0, 0);
			lgTxServo(h, 14, 1500, 50, 0, 0);
		for (int i = 0; i < 10; i++){
			driver.setPWM(i, lerp(back, test, smoothing));
			//driver.setPWM(i, test);
		}
		back = lerp(back, test, smoothing);
	
	}


	// Join Thread
		control.join(); //
		SDL_GameControllerClose(controller);
		SDL_Quit();

	lgGpiochipClose(h);
}
