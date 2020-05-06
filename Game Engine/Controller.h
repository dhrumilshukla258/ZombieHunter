#pragma once
#include "Component.h"

class Game_Object;
class Controller : public Component
{
public:
	Controller();
	~Controller();
	void HandleEvent(Event *);
	void Update();
	Component * VirtuConst();
};

