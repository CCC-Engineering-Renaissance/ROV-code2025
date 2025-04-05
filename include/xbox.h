#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>
using namespace std; 

struct POVState {
    float forward = 0.0f;
    float strafe = 0.0f;
    float vertical = 0.0f;
    float yaw = 0.0f;
    float pitch = 0.0f;
    float roll = 0.0f;
} state;

float normalize_axis(Sint16 value) {
    const float deadzone = 8000.0f; // Joystick Deadzone Preset
    if (std::abs(value) < deadzone) return 0.0f;
    return static_cast<float>(value) / 32768.0f;
}
bool running = true;

bool isRunning(){return running;}

void controls1(SDL_GameController* controller) {
    SDL_Event event;

    while (running) {
	    while (SDL_PollEvent(&event)){
	    	if (event.type == SDL_QUIT) running =false;
	    }

        // LEFT STICK: Forward/Back & Strafe
        state.forward = -normalize_axis(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY));
        state.strafe  = normalize_axis(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX));

        // RIGHT STICK: Yaw & Pitch
        state.yaw   = normalize_axis(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX));
        state.pitch = -normalize_axis(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY));

        // TRIGGERS: Vertical movement
        float lt = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT) / 32767.0f;
        float rt = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) / 32767.0f;
        state.vertical = lt - rt; // LT = Rov Rises, RT = ROV Sinks

        // BUMPERS: Roll
        const Uint8* keys = SDL_GetKeyboardState(NULL); // Optional Keyboard Fallback
        state.roll = 0.0f;
        if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
            state.roll -= 1.0f;
        if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
            state.roll += 1.0f;

        SDL_Delay(16); // 60 FPS
    }

}

