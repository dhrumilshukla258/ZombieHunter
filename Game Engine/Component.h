#pragma once
#include <stdio.h>
#include <rapidjson/document.h>
#include <fstream>
#include <sstream>
#include "Event Manager.h"
using namespace rapidjson;
enum COMPONENT_TYPE {
	TRANSFORM,
	SPRITE,
	CONTROLLER,
	UPDOWN,
	LEFTRIGHT,
	BODY,
	CAMERA,
	TEXT
};
class Event;
class Game_Object;
class Component
{
public:
	Component(unsigned int TYPE);
	virtual ~Component();
	virtual void Update() = 0;
	unsigned int GetType() { return mType; }
	virtual void Serialize( GenericObject<false, Value::ValueType> ) {}
	virtual void HandleEvent(Event *) {};
	virtual Component * VirtuConst() = 0;
public:
	Game_Object * mpOwner;
private:
	unsigned int mType;
};

