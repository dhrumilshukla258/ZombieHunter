/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Game Object.cpp
Purpose: Keeps track of all the components and game object types 
		 that are required by the GameObject
Language: C++
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: CS529_dhrumilshukla_GameEngine1.0
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Creation date: 10/15/2018
- End Header --------------------------------------------------------*/

#define _USE_MATH_DEFINES
#include <cmath> 
#include <GL/glew.h>
#include <ctime>
#include "Game Object.h"
#include "Component.h"
#include "Sprite.h"
#include "Transform.h"
#include "Controller.h"
#include "UpDown.h"
#include "Text.h"
#include "Body.h"
#include "LeftRight.h"
#include "Mesh.h"
#include "Event Manager.h"
#include "Frame Rate Controller.h"
#include "Object Factory.h"
#include "Camera.h"
#include "Physics Manager.h"
#include "Render Manager.h"

extern FrameRateController * gpFrc;
extern ObjectFactory * gpObjectFactory;
extern RenderManager * gpRenderManager;
Game_Object::Game_Object()
{
	tempTimer = 0.0f;
	mTimer = 0.0f;
	for (auto c : mpComponent)
		c = nullptr;
}

Game_Object::~Game_Object()
{
	for (auto c : mpComponent)
		delete c;
}

void Game_Object::Update() 
{	
	float frameTime = (float)gpFrc->GetFrameTime() / 1000.0f;

	//Zombie 2 Creates object after certain interval
	if (objType == ZOMBIE2) {
		mTimer -= frameTime;
		if (mTimer < 0.0f) {
			Body *pBody = static_cast<Body *> (GetComponent(BODY));
			Transform *pTr = static_cast<Transform *> (GetComponent(TRANSFORM));
			Sprite *pSp = static_cast<Sprite *> (GetComponent(SPRITE));
			//Knife Created
			if (pBody->health > 0) {
				Game_Object * pGameObject = gpObjectFactory->CreateRunTimeObject("Knife.json");
				Transform * newPtr = static_cast<Transform *> (pGameObject->GetComponent(TRANSFORM));
				Body *newPbody = static_cast<Body *> (pGameObject->GetComponent(BODY));
				pGameObject->mTimer = 15.0f;
				newPtr->mPosX = pTr->mPosX;
				newPtr->mPosY = pTr->mPosY - 20.0f;
				newPtr->mScaleX = 50.0f;
				newPtr->mScaleY = 50.0f;
				newPbody->mPosX = pTr->mPosX;
				newPbody->mPosY = pTr->mPosY - 20.0f;

				newPbody->mVelX = -45.0f;
				mTimer = 12.0f;


				if (gpRenderManager->survivalMode) {
					std::srand((unsigned int)std::time(nullptr));
					newPbody->mMass = 10.0f;
					newPbody->mInvMass = 0.1f;
					mTimer = 15.0f;
					if (pSp->isFlip) { newPbody->mVelX = (-1.0f)* (rand()%95+10); }
					else { newPbody->mVelX = (float)(rand() % 95 + 10); }

				}
			}
		}
	}

	//Delete Object After timer reaches limit
	if (objType == BULLET || objType == KNIFE || objType == CIRCLE) {
		if (mTimer < 0.0f)
			gpObjectFactory->DeleteGameObject(this);
		else
			mTimer -= frameTime;
	}
	
	if (objType == GRENADE) {
		Sprite * pSp = static_cast<Sprite *>(GetComponent(SPRITE));
		if (pSp->animationData == pSp->mapAnimationData["blast"] && 
			pSp->animationData->textureOffSetX > 0.7f && 
			pSp->animationData->textureOffSetY < 0.2f) {
			gpObjectFactory->DeleteGameObject(this);
		}
	}

	for (auto c : mpComponent)
		c->Update();
}

Component * Game_Object::AddComponent(unsigned int Type) {
	Component * pNewComponent = nullptr;
	switch (Type) {
	case TRANSFORM:
		pNewComponent = new Transform();
		break;
	case SPRITE:
		pNewComponent = new Sprite();
		break;
	case CONTROLLER:
		pNewComponent = new Controller();
		break;
	case UPDOWN:
		pNewComponent = new UpDown();
		break;
	case BODY:
		pNewComponent = new Body();
		break;
	case LEFTRIGHT:
		pNewComponent = new LeftRight();
		break;
	case CAMERA:
		pNewComponent = new Camera();
		break;
	case TEXT:
		pNewComponent = new Text();
		break;
	}

	if (nullptr != pNewComponent) {
		mpComponent.push_back(pNewComponent);
		pNewComponent->mpOwner = this;
	}

	return pNewComponent;
}

Component * Game_Object::GetComponent(unsigned int Type) {
	for (auto c : mpComponent) {
		if (c->GetType() == Type)
			return c;
	}
	return nullptr;
}

void Game_Object::HandleEvent(Event *pEvent) {
	
	for (auto c : mpComponent) {
			c->HandleEvent(pEvent);
	}
}