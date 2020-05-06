#pragma once
#include "Component.h"
#include "Game Object.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera : public Component
{
public:
	Camera();
	glm::mat4 GetView();
	~Camera();
	void Update();
	Component * VirtuConst();
	void Serialize(GenericObject<false, Value::ValueType> );
};

