/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Render Manager.cpp
Purpose: Render Objects, Call Text Rendering, Renders Bounding Boxes
Language: C++
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: CS529_dhrumilshukla_GameEngine1.0
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Creation date: 11/18/2018
- End Header --------------------------------------------------------*/

#define _USE_MATH_DEFINES

#include "Render Manager.h"
#include "Shader.h"
#include "Game Object Manager.h"
#include "Game Object.h"
#include "Transform.h"
#include "Sprite.h"
#include "Collision Manager.h"
#include <math.h>
#include <iostream>
#include "Vector2D.h"
#include "Physics Manager.h"
#include "Text.h"
#include "Resource Manager.h"
#include "Object Factory.h"
extern Game_Object_Manager * gpGameObjectManager;
extern ObjectFactory * gpObjectFactory;
extern PhysicsManager * gpPhysicsManager;
extern bool isMenu;
extern bool isDebug;
extern ResourceManager * gpResourceManager;
glm::mat4 p = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);

RenderManager::RenderManager()
{
	Shader s;
	shaderColor = s.initialize("shaderColor.vs","shaderColor.fs");
	shaderTexture = s.initialize("shaderTexture.vs","shaderTexture.fs");
	isTexture = true;
	view = { 1,0,0,0,
			 0,1,0,0,
			 0,0,1,0,
			 0,0,0,1 };
	view1 = { 1,0,0,0,
			 0,1,0,0,
			 0,0,1,0,
			 0,0,0,1 };

	mTextRenderer = new TextRenderer();

	VAOCircle = gpResourceManager->LoadMesh("Circle");
	VAOLine = gpResourceManager->LoadMesh("Line");
}

RenderManager::~RenderManager()
{
	delete mTextRenderer;
}

void RenderManager::Update() {
	ReUse(gpGameObjectManager->mGameObject);

	for (auto go : gpGameObjectManager->mGameObject) {

		if (go->objType == PLAYER) {
			Body * pBody = static_cast<Body *> (go->GetComponent(BODY));
			glm::vec3 color(1.0f, 1.0f, 0.8f);
			std::string heal = std::to_string(pBody->health);
			mTextRenderer->RenderText("Health - "+heal, 10, 550, 1, color);
			std::string kill = std::to_string(gpObjectFactory->deadZombie);
			mTextRenderer->RenderText("Kills - " + kill, 650, 550, 1, color);

			//mTextRenderer->RenderText(heal, 100, 550, 1, color);
			break;
		}
	}
}

void RenderManager::MenuUpdate() {
	ReUse(gpGameObjectManager->mMenuObject);
}

void RenderManager::ReUse( std::vector<Game_Object *> temp) {
	
	for (auto go : temp)
	{
		Transform * pTr = static_cast<Transform *>(go->GetComponent(TRANSFORM));
		Sprite * pSp = static_cast<Sprite *>(go->GetComponent(SPRITE));
		if (pTr && pSp) {
			float textureOffSetX = 0.0f;
			float textureOffSetY = 0.0f;
			float mulTextureX = 1.0f;
			float mulTextureY = 1.0f;
			float temp = 1.0f;

			float x = pTr->mPosX;		
			float y = pTr->mPosY;
			GLuint VAO = pSp->VAO;		
			GLuint texture = pSp->texture;		
			SpriteAnimationData * ani = pSp->animationData;

			glm::vec3 v(x, y, 0.0f);
			glm::mat4 transform;			
			
			if (nullptr != ani) {
				mulTextureX = ani->mulTextureX;
				mulTextureY = ani->mulTextureY;
				textureOffSetX = ani->textureOffSetX;
				textureOffSetY = ani->textureOffSetY;
				if (pSp->isFlip) {
					temp = -1.0f;
				}
			}
			glm::vec3 bloodeffect = {1.0f,1.0f,1.0f};
			if (go->isHit) {
				go->isHit = false;
				bloodeffect = { 0.95,0.1f,0.1f };
			}

			transform = glm::translate(transform, v)
				* glm::rotate(transform, pTr->mAngle, glm::vec3(0.0f, 0.0f, 1.0f))
				* glm::scale(transform, glm::vec3(temp * pTr->mScaleX, pTr->mScaleY, 0.0f));

			glm::mat4 projection;
			if (isMenu == true) {
				projection = p * view1;
			}
			else {
				projection = p * view;
			}
			
			if (survivalMode) {
				projection = p;
			}

			if (texture && isTexture) {
				glUseProgram(shaderTexture);
				unsigned int utransform = glGetUniformLocation(shaderTexture, "transform");
				unsigned int uprojection = glGetUniformLocation(shaderTexture, "projection");
				unsigned int ublood = glGetUniformLocation(shaderTexture, "blood");
				unsigned int utextureOffSet = glGetUniformLocation(shaderTexture, "textureOffSet");
				unsigned int umulTexture = glGetUniformLocation(shaderTexture, "mulTexture");
				glUniform2f(umulTexture, mulTextureX, mulTextureY);
				glUniform2f(utextureOffSet, textureOffSetX, textureOffSetY);
				glUniform3f(ublood, bloodeffect.x, bloodeffect.y, bloodeffect.z);
				glUniformMatrix4fv(utransform, 1, GL_FALSE, glm::value_ptr(transform));
				glUniformMatrix4fv(uprojection, 1, GL_FALSE, glm::value_ptr(projection));
				glBindTexture(GL_TEXTURE_2D, texture);

			}
			else {
				glUseProgram(shaderColor);
				unsigned int utransform = glGetUniformLocation(shaderColor, "transform");
				unsigned int uprojection = glGetUniformLocation(shaderColor, "projection");
				glUniformMatrix4fv(utransform, 1, GL_FALSE, glm::value_ptr(transform));
				glUniformMatrix4fv(uprojection, 1, GL_FALSE, glm::value_ptr(projection));

			}

			// draw our first triangle
			glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		Text * pText = static_cast<Text *> (go->GetComponent(TEXT));
		if (nullptr != pText) {
				mTextRenderer->RenderText(
				pText->whatsTheText,
				pText->mTextPosX,
				pText->mTextPosY,
				pText->scale, pText->color);
			//mTextRenderer->RenderText("Controls:", 500, 250, 1, color);
		}

		if (isDebug && !isMenu) {
			Body * pBody = static_cast<Body *>(go->GetComponent(BODY));
			float radius = 0.0f;
			if (pBody) {
				float scaleX = 0.0f, scaleY = 0.0f;
				Shape * sP = pBody->mpShape;
				if (sP->mType == Shape::RECTANGLE) {
					ShapeAABB * s = static_cast<ShapeAABB *> (sP);
					scaleX = s->mBottom;
					scaleY = s->mLeft;
				}
				else if (sP->mType == Shape::LINESEGMENT) {
					ShapeLineSegment * s = static_cast<ShapeLineSegment *> (sP);
					scaleX = pTr->mScaleX;
					scaleY = pTr->mScaleY;
				}
				else if (sP->mType == Shape::CIRCLE) {
					ShapeCircle * s = static_cast<ShapeCircle *> (sP);
					scaleX = s->mRadius;
					scaleY = s->mRadius;
					radius = 1.0f;

				}

				float x = pBody->mPosX;
				float y = pBody->mPosY;
				GLuint VAO = pSp->VAO;

				glm::vec3 v(x, y, 0.0f);
				glm::mat4 transform;

				transform = glm::translate(transform, v)
					* glm::rotate(transform, pTr->mAngle, glm::vec3(0.0f, 0.0f, 1.0f))
					* glm::scale(transform, glm::vec3(scaleX, scaleY, 0.0f));

				glm::mat4 projection;

				if (survivalMode) {
					projection = p;
				}
				else {
					projection = p * view;
				}

				glUseProgram(shaderColor);
				unsigned int utransform = glGetUniformLocation(shaderColor, "transform");
				unsigned int uprojection = glGetUniformLocation(shaderColor, "projection");
				glUniformMatrix4fv(utransform, 1, GL_FALSE, glm::value_ptr(transform));
				glUniformMatrix4fv(uprojection, 1, GL_FALSE, glm::value_ptr(projection));

				

				if (radius == 1.0f || go->objType == GRENADE) {
					glBindVertexArray(VAOCircle);
					glDrawArrays(GL_LINE_LOOP, 0, 16);
				}
				else {
					glBindVertexArray(VAO);
					glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
				}


				float ang;
				if (pBody->mVelY != 0.0f || pBody->mVelX != 0.0f) {
					ang = atan2f(pBody->mVelY, pBody->mVelX);
					glm::mat4 t(1);
					t = glm::translate(t, v)
						* glm::rotate(t, ang, glm::vec3(0.0f, 0.0f, 1.0f))
						* glm::scale(t, glm::vec3(100, 100, 0.0f));
					glUniformMatrix4fv(utransform, 1, GL_FALSE, glm::value_ptr(t));
					glBindVertexArray(VAOLine);
					glDrawArrays(GL_LINE_STRIP, 0, 2);

				}
			}
		}

	}

}

void RenderManager::Background(unsigned int texture, unsigned int VAO) {

	glm::mat4 transform;
	
	glm::mat4 projection = p *view;

	if (survivalMode) {
		transform =
			glm::translate(transform, glm::vec3(400.0f, 300.0f, 0.0f)) *
			glm::scale(transform, glm::vec3(800, 600, 0.0f));
		projection = p;
	}
	else {
		transform =
			glm::translate(transform, glm::vec3((mCamX*-1.0f) + 400.0f, mCamY + 300.0f, 0.0f)) *
			glm::scale(transform, glm::vec3(800, 600, 0.0f));
	}
	glUseProgram(shaderTexture);
	glm::vec3 bloodeffect = { 1.0f,1.0f,1.0f };
	unsigned int ublood = glGetUniformLocation(shaderTexture, "blood");
	glUniform3f(ublood, bloodeffect.x, bloodeffect.y, bloodeffect.z);
	unsigned int utransform = glGetUniformLocation(shaderTexture, "transform");
	unsigned int uprojection = glGetUniformLocation(shaderTexture, "projection");
	unsigned int utextureOffSet = glGetUniformLocation(shaderTexture, "textureOffSet");
	unsigned int umulTexture = glGetUniformLocation(shaderTexture, "mulTexture");
	glUniform2f(umulTexture, 1, 1);
	glUniform2f(utextureOffSet, 0, 0);
	glUniformMatrix4fv(utransform, 1, GL_FALSE, glm::value_ptr(transform));
	glUniformMatrix4fv(uprojection, 1, GL_FALSE, glm::value_ptr(projection));
	glBindTexture(GL_TEXTURE_2D, texture);

	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}