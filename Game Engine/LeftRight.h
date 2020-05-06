#pragma once
#include "Component.h"
class LeftRight : public Component
{
public:
	LeftRight();
	~LeftRight();
	void Update();
	void Serialize(GenericObject<false, Value::ValueType>);
	void HandleEvent(Event * pEvent);
	Component * VirtuConst();
	bool mGoingRight;
	int mTimer, mTimerLimit;
	float mSpeed;
};

