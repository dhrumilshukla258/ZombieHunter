#pragma once
#include "Component.h"
#include "Game Object.h"
#include <glm/glm.hpp>
class Text : public Component
{
public:
	Text();
	~Text();
	void Update();
	Component * VirtuConst();
	void Serialize(GenericObject<false, Value::ValueType>);
	void HandleEvent(Event * pEvent);
public:
	std::string whatsTheText;
	glm::vec3 color;
	float time;
	float scale;
	std::string counter;
	float mTextPosX, mTextPosY;
};

