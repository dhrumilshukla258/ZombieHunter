/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Frame Rate Controller.cpp
Purpose: Implement class function of frame rate controller
Language: C++
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: CS529_dhrumilshukla_GameEngine1.0
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Creation date: 10/15/2018
- End Header --------------------------------------------------------*/

#include "Frame Rate Controller.h"
#include <SDL2.0/SDL_timer.h>

FrameRateController::FrameRateController(Uint32 MaxFrameRate) {
	mTickStart = mTickEnd = mFrameTime = 0;

	if (0 != MaxFrameRate)
		mNeededTicksPerFrame = 1000 / MaxFrameRate;
	else 
		mNeededTicksPerFrame = 0;
}

FrameRateController::~FrameRateController() {
}

void FrameRateController::FrameStart() {
	mTickStart = SDL_GetTicks();
}

void FrameRateController::FrameEnd() {
	mTickEnd = SDL_GetTicks();
	while ((mTickEnd - mTickStart < mNeededTicksPerFrame))
		mTickEnd = SDL_GetTicks();

	mFrameTime = mTickEnd - mTickStart;
}

Uint32 FrameRateController::GetFrameTime() {
	Uint32 x;
	if (mFrameTime == 0) 
		x = 0;
	else 
		x = 1000 / mFrameTime;
	return x;
}