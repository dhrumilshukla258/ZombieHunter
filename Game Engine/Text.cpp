/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Text.cpp
Purpose: Implements Text component of GameObject
Language: C++
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: CS529_dhrumilshukla_GameEngine1.0
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Creation date: 11/28/2018
- End Header --------------------------------------------------------*/

#include "Text.h"

Text::Text() : Component(TEXT)
{
	color = {0.0f, 0.0f, 0.0f};
}


Text::~Text()
{
}

void Text::Update() {

}

void Text::Serialize(GenericObject<false, Value::ValueType> t) {
	color = { t["color"]["r"].GetFloat(),
		t["color"]["g"].GetFloat(),
		t["color"]["b"].GetFloat()
	};
	whatsTheText = t["whatsTheText"].GetString();
	scale = t["scale"].GetFloat();
	time = t["time"].GetFloat();
	mTextPosX = t["x"].GetFloat();
	mTextPosY = t["y"].GetFloat();
}

Component * Text::VirtuConst() {
	return new Text(*this);
}

void Text::HandleEvent(Event * pEvent){}