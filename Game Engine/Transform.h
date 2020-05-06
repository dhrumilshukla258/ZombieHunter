#pragma once
#include "Component.h"
#include "Game Object.h"
#include <iostream>
#include "Body.h"
class Transform : public Component
{
public:
	Transform();
	~Transform();
	void Update();
	void Serialize( GenericObject<false, Value::ValueType> );
	Component * VirtuConst();
public:
	float mPosX;
	float mPosY;
	float mScaleX;
	float mScaleY;
	float isOffset;
	float mAngle;
};

