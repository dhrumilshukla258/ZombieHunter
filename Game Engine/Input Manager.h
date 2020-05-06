#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <SDL2.0/SDL_stdinc.h>

#include <SDL2.0/SDL.h>
#include <SDL2.0/SDL_Keyboard.h>

class InputManager {
public:
	InputManager();
	~InputManager();

	bool Update();
	bool IsPressed(unsigned int KeyScanCode); //From SDL_ScanCode
	bool IsTriggerred(unsigned int KeyScanCode);
	bool IsReleased(unsigned int KeyScanCode);
	bool IsMouseButtonTriggered(unsigned int);

private:
	Uint8 mCurrentState[512];
	Uint8 mPreviousState[512];
	bool mCurrentMouseState[3];
	bool mPreviousMouseState[3];
};


#endif