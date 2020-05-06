/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Main.cpp
Purpose: Include all major functions
Language: C++
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: CS529_dhrumilshukla_GameEngine1.0
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Creation date: 10/15/2018
- End Header --------------------------------------------------------*/

#define SDL_MAIN_HANDELED
#include <SDL2.0/SDL.h>
#include <stdio.h>
#include <Windows.h>

#include "Input Manager.h"
#include "Resource Manager.h"
#include "Frame Rate Controller.h"
#include "Game Object Manager.h"
#include "Object Factory.h"
#include "Render Manager.h"
#include "Physics Manager.h"
#include "Collision Manager.h"
#include "Event Manager.h"
#include "Game Object.h"

#include "GL/glew.h"

//GameStop Variable
bool appIsRunning = true;
bool isGameStopped = false;
bool isGameDebug;
bool isMenu = true;
bool isDebug = false;
float frameTime = 0.016f;

//InputManager
InputManager * gpInputManager = new InputManager();
//ResourceManager
ResourceManager * gpResourceManager = new ResourceManager();
//FrameRateControler
FrameRateController * gpFrc = new FrameRateController(60);
//GameObjectManager
Game_Object_Manager * gpGameObjectManager = new Game_Object_Manager();
//ObjectFactory
ObjectFactory * gpObjectFactory = new ObjectFactory();
//PhysicsManager
PhysicsManager * gpPhysicsManager = new PhysicsManager();
//CollisionManager
CollisionManager * gpCollisionManager = new CollisionManager();
//Render Manager
RenderManager * gpRenderManager = nullptr;
//Event Manager
EventManager * gpEventManager = new EventManager();

FILE _iob[] = { *stdin, *stdout, *stderr };
extern "C" FILE * __cdecl __iob_func(void)
{
	return _iob;
}

#pragma comment(lib, "legacy_stdio_definitions.lib")

int main(int argc, char* args[])
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_Window *pWindow;
	int error = 0;

	// Initialize SDL
	if((error = SDL_Init( SDL_INIT_VIDEO )) < 0 )
	{
		printf("Couldn't initialize SDL, error %i\n", error);
		return 1;
	}
	pWindow = SDL_CreateWindow("SDL Window",		// window title
		SDL_WINDOWPOS_UNDEFINED,					// initial x position
		SDL_WINDOWPOS_UNDEFINED,					// initial y position
		800,										// width, in pixels
		600,										// height, in pixels
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	// Check that the window was successfully made
	if (NULL == pWindow)
	{
		// In the event that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}
	SDL_GLContext context = SDL_GL_CreateContext(pWindow);

	//Shader Initialization Also happens in RenderManager
	RenderManager r;
	gpRenderManager = &r;
	gpObjectFactory->LoadLevel("NewGameMenu.json", isMenu);
	//gpObjectFactory->LoadMultipleLevel("Level.json");
	
	// Game loop
	while (true == appIsRunning)
	{
		glClearColor(1,1,1,1);
		glClear(GL_COLOR_BUFFER_BIT);
		gpFrc->FrameStart();
		isGameDebug = false;
		char buffer[30];
		frameTime = (float)gpFrc->GetFrameTime();
		sprintf_s(buffer, "Dhrumil | Frames:- %.2f", frameTime);
		SDL_SetWindowTitle(pWindow, buffer);

		appIsRunning = gpInputManager->Update();
		
		if ( ( !isGameStopped || isGameDebug ) && !isMenu) {
			
			gpPhysicsManager->Update(frameTime / 1000.0f);
			gpEventManager->Update(frameTime / 1000.0f);
			gpObjectFactory->Update();
			//Update Game Objects
			for (auto go : gpGameObjectManager->mGameObject)
				go->Update();
		}
		
		gpRenderManager->Background(gpObjectFactory->textureBackground, gpObjectFactory->VAOBackground);
		gpRenderManager->Update();

		if (isMenu) {
			gpPhysicsManager->MenuUpdate(frameTime);
			//Update Menu Objects
			for (auto go : gpGameObjectManager->mMenuObject)
				go->Update();


			gpRenderManager->Background(gpObjectFactory->textureBackgroundBlur, gpObjectFactory->VAOBackgroundBlur);
			gpRenderManager->MenuUpdate();
		}
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		SDL_GL_SwapWindow(pWindow);
		glViewport(0, 0, 800, 600);
		gpFrc->FrameEnd();
	}
	
	//Deletion of Manager
	delete gpInputManager;
	delete gpFrc;
	delete gpResourceManager;
	delete gpGameObjectManager;
	delete gpObjectFactory;
	delete gpCollisionManager;
	delete gpPhysicsManager;
	//delete gpRenderManager;
	delete gpEventManager;
	// Close and destroy the window
	SDL_DestroyWindow(pWindow);

	// Quit SDL subsystems
	SDL_Quit();
	
	return 0;
}