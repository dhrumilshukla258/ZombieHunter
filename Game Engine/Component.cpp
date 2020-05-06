/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Component.cpp
Purpose: Keeps track of each Component of GameObject
Language: C++
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: CS529_dhrumilshukla_GameEngine1.0
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Creation date: 10/25/2018
- End Header --------------------------------------------------------*/

#include "Component.h"

Component::Component(unsigned int Type)
{
	mpOwner = nullptr;
	mType = Type;
}

Component::~Component()
{
}
