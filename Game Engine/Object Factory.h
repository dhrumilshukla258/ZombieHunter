#pragma once
#include <rapidjson/document.h>
#include <unordered_map>
using namespace rapidjson;

class Game_Object;
class ObjectFactory
{
public:
	ObjectFactory();
	~ObjectFactory();
	Game_Object * LoadObject(const char *);
	void LoadLevel(const char *, bool isMenu);
	Game_Object * CreateRunTimeObject(std::string );
	Document loadDocument(std::string fullPath);
	void type(std::string objTypeString, Game_Object * pNewGameObject);
	void DeleteGameObject(Game_Object *);
	void Update();
	//Document LoadSprite ( std::string );
	//void LoadModel(const char *pFileName);
public:
	std::unordered_map< std::string, Game_Object *> objectList;
	std::vector<Game_Object *> runTimeObjects;
	std::vector<Game_Object *> deleteObject;
	unsigned int textureBackground, textureBackgroundBlur;
	unsigned int VAOBackground, VAOBackgroundBlur;
	bool whichZombie;
	bool walkZombie;
	bool bombCreated = false;
	int bomb;
	bool leftRightZombie;
	int liveZombie;
	int deadZombie;
};

