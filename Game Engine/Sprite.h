#pragma once
#include "Component.h"
#include "Mesh.h"
#include <unordered_map>
#include "Game Object.h"
class Mesh;

class SpriteAnimationData {
public:
	int numberOfSpritesInAnImage;
	int tempForLoopingPurpose;
	float textureOffSetX, textureOffSetY;
	float mulTextureX, mulTextureY;
	float time_limit;
	bool isLoop;
	SpriteAnimationData();
	~SpriteAnimationData();
};

class AnotherBody : public Game_Object {
	AnotherBody() {}
	~AnotherBody() {}
};

class Sprite : public Component
{
public:
	Sprite();
	~Sprite();
	void Update();
	void Serialize( GenericObject<false, Value::ValueType> );
	Component * VirtuConst();
public:
	GLuint VAO;
	GLuint texture;
	AnotherBody * mAnotherBody;
	std::unordered_map<std::string, GLuint> mapTexture;
	SpriteAnimationData * animationData;
	std::unordered_map<std::string, SpriteAnimationData *> mapAnimationData;
	bool isFlip = false;
	bool isCollide = false;
	float current_time = 0.0f;
};

