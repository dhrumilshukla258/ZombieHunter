#pragma once
#include <vector>
#include "Game Object.h"
#include "TextRenderer.h"
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class GameObjectManager;
class RenderManager
{
public:
	RenderManager();
	~RenderManager();
	void Update();
	void MenuUpdate();
	void ReUse( std::vector<Game_Object *> );
	void Background(unsigned int, unsigned int);
//	unsigned int * CreateTexture(const char * textureFile);
	
	TextRenderer * mTextRenderer;

	bool isTexture;
	glm::mat4 view;
	glm::mat4 view1;
	bool survivalMode = false;
	float mCamX = 0.0f;
	float mCamY= 0.0f;
	GLuint VAOCircle;
	GLuint VAOLine;
private:
	int shaderColor;
	int shaderTexture;
};
