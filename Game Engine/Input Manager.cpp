/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Input Manager.cpp
Purpose: Implement Input Manager class functions using SDL
Language: C++
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: CS529_dhrumilshukla_GameEngine1.0
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Creation date: 10/15/2018
- End Header --------------------------------------------------------*/

#include "Input Manager.h"
#include "Object Factory.h"
#include "Physics Manager.h"
#include <iostream>

//GameStop Variable
extern bool isGameStopped;
extern bool isGameDebug;
extern float frameTime;
extern bool isMenu;

extern ObjectFactory * gpObjectFactory;

InputManager::InputManager() {
	SDL_memset(mCurrentState, 0, 512*sizeof(Uint8));
	SDL_memset(mPreviousState, 0, 512 * sizeof(Uint8));
	SDL_memset(mCurrentMouseState, false, 3 * sizeof(bool));
	SDL_memset(mPreviousMouseState, false, 3 * sizeof(bool));
}

InputManager::~InputManager() {
}

bool InputManager::Update() {
	SDL_Event e;
	bool pMouseState[3];
	while (SDL_PollEvent(&e) != 0 )
	{
		//User requests quit
		if (e.type == SDL_QUIT)
			return false;

		else if (e.type == SDL_MOUSEBUTTONDOWN) {
			pMouseState[e.button.button - 1] = true;
		}

		else if (e.type == SDL_MOUSEBUTTONUP) {
			pMouseState[e.button.button - 1] = false;
		}
	}
	if (IsTriggerred(SDL_SCANCODE_ESCAPE)) {
		if (!isMenu) {
			isGameStopped = true;
			isMenu = true;
			gpObjectFactory->LoadLevel("PauseMenu.json", true);
		}
	}
	if (!isGameStopped) {
		if (IsTriggerred(SDL_SCANCODE_LEFT)) {
			isGameStopped = true;
		}
	}
	else {
		if (IsTriggerred(SDL_SCANCODE_RIGHT)) {
			isGameStopped = false;
		}

		else
			if (IsTriggerred(SDL_SCANCODE_LEFT)) {
				frameTime = 0.016f;
				isGameDebug = true;
			}
	}

	SDL_memcpy(mPreviousMouseState, mCurrentMouseState, 3 * sizeof(bool));
	SDL_memcpy(mCurrentMouseState, pMouseState, 3 * sizeof(bool));
	

	int numberOfFetchedkeys = 0;
	const Uint8* pCurrentKeyStates = SDL_GetKeyboardState(&numberOfFetchedkeys);
	if (numberOfFetchedkeys > 512)
		numberOfFetchedkeys = 512;

	SDL_memcpy(mPreviousState, mCurrentState, 512 * sizeof(Uint8));
	SDL_memcpy(mCurrentState, pCurrentKeyStates, numberOfFetchedkeys * sizeof(Uint8));
	
	return true;
/*
	int *y = nullptr;
	int *y = nullptr;
	Uint32 pMouseCurrentStates = SDL_GetMouseState(x, y);

	SDL_memcpy(mMousePreviousState, mMouseCurrentState, 20 * sizeof(Uint32));
	SDL_memcpy(mMouseCurrentState, pMouseCurrentStates, 20 * sizeof(Uint32));*/

}

bool InputManager::IsPressed(unsigned int KeyScanCode) { //From SDL_ScanCode
	if (KeyScanCode >= 512)
		return false;
	if (mCurrentState[KeyScanCode])
		return true;

	return false;
}
bool InputManager::IsMouseButtonTriggered(unsigned int x) {
	if (mCurrentMouseState[x - 1] && !mPreviousMouseState[x - 1])
		return true;
	
	return false;
}
bool InputManager::IsTriggerred(unsigned int KeyScanCode) {
	if (KeyScanCode >= 512)
		return false;
	if (mCurrentState[KeyScanCode] && !mPreviousState[KeyScanCode])
		return true;
	
	return false;
}
bool InputManager::IsReleased(unsigned int KeyScanCode) {
	if (KeyScanCode >= 512)
		return false;
	if (!mCurrentState[KeyScanCode] && mPreviousState[KeyScanCode])
		return true;

	return false;
}