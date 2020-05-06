#pragma once
#include <vector>

class Game_Object;
class Game_Object_Manager
{
public:
	Game_Object_Manager();
	~Game_Object_Manager();
public:
	std::vector<Game_Object *> mGameObject;
	std::vector<Game_Object *> mMenuObject;
};

