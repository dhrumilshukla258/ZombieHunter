/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Transform.cpp
Purpose: Implements Transform component of GameObject
Language: C++
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: CS529_dhrumilshukla_GameEngine1.0
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Creation date: 10/15/2018
- End Header --------------------------------------------------------*/

#include "Transform.h"
#include "Resource Manager.h"
#define M_PI 3.14159265358979323846264338327950288

extern ResourceManager *gpResourceManager;

Transform::Transform() : Component(TRANSFORM)
{
	mPosX = mPosY = 0.0f;
	mAngle = 0.0f;
	isOffset = 0;
}

Transform::~Transform()
{
}

void Transform::Update() {
	if (mpOwner->objType == KNIFE) {
		mAngle += 0.1f;
		if (mAngle > 2 * 3.1415f)
			mAngle = 0.0f;
	}
	
}

void Transform::Serialize(GenericObject<false, Value::ValueType> t)
{
	mPosX = t["posx"].GetFloat();
	mPosY = t["posy"].GetFloat();
	mScaleX = t["scalex"].GetFloat();
	mScaleY = t["scaley"].GetFloat();
	if (t.HasMember("angle")) {
		mAngle = ( t["angle"].GetFloat() * (float)M_PI )/180.0f ;
	}
}

Component * Transform::VirtuConst() {
	return new Transform(*this);
}