#pragma once
#include "Component.h"
#include "Game Object.h"
class Shape;
class Body : public Component
{
public:
	Body();
	~Body();
	void Update();
	void Serialize(GenericObject<false, Value::ValueType>);
	void Initialize();
	void Integrate( float DeltaTime);
	Component * Body::VirtuConst();
	void HandleEvent(Event * pEvent);
public:
	float mPosX, mPosY;
	float mPrevPosX, mPrevPosY;
	float mVelX, mVelY;
	float mAccX, mAccY;
	float mTotalForceX, mTotalForceY;
	float mMass, mInvMass;
	int health = 100;
	bool isFloorCreated = false;
	Shape * mpShape;
};

