/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Controller.cpp
Purpose: Implements Controller component of GameObject
Language: C++
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: CS529_dhrumilshukla_GameEngine1.0
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Creation date: 10/25/2018
- End Header --------------------------------------------------------*/

#include <SDL2.0/SDL_scancode.h>

#include <SDL2.0/SDL.h>
#include "Controller.h"
#include "Input Manager.h"
#include "Game Object.h"
#include "Transform.h"
#include "Body.h"
#include "Collision Manager.h"
#include "Physics Manager.h"
#include "Event Manager.h"
#include "Sprite.h"
#include "Object Factory.h"
#include "Render Manager.h"
#include "Camera.h"

#define force 120.0f;
#define jumpforce 10000.0f;
extern InputManager * gpInputManager;
extern EventManager * gpEventManager;
extern ObjectFactory * gpObjectFactory;
extern RenderManager * gpRenderManager;
//
int weapon = 1;
bool isWeapon = false;

extern bool isMenu;
extern bool isDebug;
Controller::Controller() : Component(CONTROLLER)
{
}

Controller::~Controller()
{
}

void Controller::Update() {
	if (nullptr != mpOwner && nullptr != gpInputManager) {
		
		Body *pBody = static_cast<Body *> (mpOwner->GetComponent(BODY));
		Transform *pTr = static_cast<Transform *> (mpOwner->GetComponent(TRANSFORM));
		Sprite *pSp = static_cast<Sprite *> (mpOwner->GetComponent(SPRITE));
		
		if (nullptr != pTr) {
			if (isMenu) {
				if (gpInputManager->IsMouseButtonTriggered(SDL_BUTTON_LEFT)) {
					int x, y;
					SDL_GetMouseState(&x, &y);
					pBody->mPosX = (float)x;
					pBody->mPosY = 600.0f - (float)y;
				}
			}			
			else {
				if (gpInputManager->IsPressed(SDL_SCANCODE_1)) {
					weapon = 1;

				}
				if (gpInputManager->IsPressed(SDL_SCANCODE_2)) {
					weapon = 2;
				}
				if (gpInputManager->IsMouseButtonTriggered(SDL_BUTTON_LEFT)) {
					Game_Object * pGameObject = nullptr;
					pTr->isOffset = 1;
					if (weapon == 1) {
						pGameObject = gpObjectFactory->CreateRunTimeObject("Bullet.json");
						pSp->animationData = pSp->mapAnimationData["attackbullet"];
						pSp->texture = pSp->mapTexture["attackbullet"];
					}
					if (weapon == 2) {
						pGameObject = gpObjectFactory->CreateRunTimeObject("Circle.json");
						pSp->animationData = pSp->mapAnimationData["attackcircle"];
						pSp->texture = pSp->mapTexture["attackcircle"];
					}
					pGameObject->mTimer = 18.0f;
					Transform * newPtr = static_cast<Transform *> (pGameObject->GetComponent(TRANSFORM));
					Body *newPbody = static_cast<Body *> (pGameObject->GetComponent(BODY));

					newPtr->mPosX = pTr->mPosX;		newPtr->mPosY = pTr->mPosY - 5.0f;
					newPtr->mScaleX = 10.0f;				newPtr->mScaleY = 10.0f;
					newPbody->mPosX = pTr->mPosX;		newPbody->mPosY = pTr->mPosY - 5.0f;

					int x, y;
					SDL_GetMouseState(&x, &y);
					y = 600 - y;

					if (!gpRenderManager->survivalMode) {
						x += (int)(gpRenderManager->mCamX * -1.0f);
					}


					x -= (int)pTr->mPosX;
					y -= (int)pTr->mPosY;

					newPtr->mAngle = atan2f((float)y, (float)x);

					float angle = newPtr->mAngle;
					newPbody->mVelX = 100.0f * cosf(angle);
					newPbody->mVelY = 100.0f * sinf(angle);


					if (newPbody->mVelX < 0) {
						pSp->isFlip = true;
					}
					else {
						pSp->isFlip = false;
					}
				}
				if ((gpInputManager->IsTriggerred(SDL_SCANCODE_W) || gpInputManager->IsTriggerred(SDL_SCANCODE_SPACE)) && pBody->mVelY == 0) {
					pBody->mTotalForceY += jumpforce;

					pTr->isOffset = 1;
					pSp->texture = pSp->mapTexture["jump"];
					pSp->animationData = pSp->mapAnimationData["jump"];
				}
				if (gpInputManager->IsPressed(SDL_SCANCODE_S)) {
					pBody->mTotalForceY -= force;
				}
				if (gpInputManager->IsPressed(SDL_SCANCODE_A)) {
					pBody->mTotalForceX -= force;
					if (nullptr != pSp && pSp->texture != pSp->mapTexture["jump"]) {
						if (!pSp->isFlip) {
							pSp->isFlip = !pSp->isFlip;
						}
						pSp->texture = pSp->mapTexture["walk"];
						pSp->animationData = pSp->mapAnimationData["walk"];
					}
				}
				if (gpInputManager->IsPressed(SDL_SCANCODE_D)) {
					pBody->mTotalForceX += force;
					pTr->isOffset = 1;
					if (nullptr != pSp && pSp->texture != pSp->mapTexture["jump"]) {
						if (pSp->isFlip) {
							pSp->isFlip = !pSp->isFlip;
						}
						pSp->texture = pSp->mapTexture["walk"];
						pSp->animationData = pSp->mapAnimationData["walk"];
					}
				}
				if (gpInputManager->IsTriggerred(SDL_SCANCODE_U)) {
					isDebug = !isDebug;
				}
			}
		}
	}
}

void Controller::HandleEvent(Event * pEvent) {
	if (COLLIDE == pEvent->mType) {
		CollideEvent * pReadEvent = static_cast <CollideEvent *>(pEvent);

		//gpEventManager->Subscribe(pReadEvent->mType, mpOwner);
		Body *pBody = static_cast <Body *>(mpOwner->GetComponent(BODY));
		if (nullptr != pBody) {

		}

		//PlayerHitEvent pHe;
		//gpEventManager->BroadcastEvent(&pHe);

		/*PlayerHitEvent * pPhe;
		pPhe = new PlayerHitEvent;
		pPhe->mTimer = 3.0f;
		gpEventManager->AddTimedEvent(pPhe);*/
		
	}

}

Component * Controller::VirtuConst() {
	return new Controller(*this);
}