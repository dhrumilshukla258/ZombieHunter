#pragma once
#include "Component.h"

class UpDown : public Component
{
public:
	UpDown();
	~UpDown();
	void Update();
	void Serialize( GenericObject<false, Value::ValueType> );
	void HandleEvent(Event * pEvent);
	Component * VirtuConst();
public:
	bool mGoingUp;
	int mTimer, mTimerLimit;
	float mSpeed;
	bool isStart;
};

