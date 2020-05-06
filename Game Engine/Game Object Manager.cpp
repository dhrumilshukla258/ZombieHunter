/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Game Object Manager.cpp
Purpose: Manages Menu and Game Objects
Language: C++
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: CS529_dhrumilshukla_GameEngine1.0
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Creation date: 10/15/2018
- End Header --------------------------------------------------------*/

#include "Game Object Manager.h"
#include "Game Object.h"

Game_Object_Manager::Game_Object_Manager()
{
}


Game_Object_Manager::~Game_Object_Manager()
{
	for (auto go : mGameObject)
		delete go;
	mGameObject.clear();
}
