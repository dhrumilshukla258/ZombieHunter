#pragma once
#include <vector>

enum GAME_OBJECT_TYPE {
	PLAYER,
	FLOOR,
	CRATE,
	LIFT,
	LIFT3,
	PRESSKEY1,
	PRESSKEY2,
	PRESSKEY3,
	ZOMBIE1,
	ZOMBIE2,
	ZOMBIERUN,
	BULLET,
	MISSILE,
	KNIFE,
	CIRCLE,
	LINESEGMENT,
	NOCOLLISIONTHING,


	NEWBUTTON,
	QUITBUTTON,
	CONTROLBUTTONMAIN,
	CONTROLBUTTONPAUSE,
	MOUSECLICKINMENU,
	BACKBUTTON,
	BACKBUTTONCONTROLMAIN,
	BACKBUTTONCONTROLPAUSE,
	CREDITBUTTONMAIN,
	CAMPAIGN,
	SURVIVAL,
	GRENADE
};


class Event;
class Component;
class Game_Object
{
public:
	Game_Object();
	~Game_Object();

	void Update();
	Component * AddComponent(unsigned int Type);
	Component * GetComponent(unsigned int Type);
	void HandleEvent(Event *pEvent);
public:
	std::vector<Component *> mpComponent;
	unsigned int objType;
	bool isHit = false;
	float mTimer;
	float tempTimer;
};

