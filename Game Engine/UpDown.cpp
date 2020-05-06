/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Body.cpp
Purpose: Implements UpDown component of GameObject
Language: C++
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: CS529_dhrumilshukla_GameEngine1.0
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Creation date: 10/15/2018
- End Header --------------------------------------------------------*/

#include "UpDown.h"
#include "Frame Rate Controller.h"
#include "Transform.h"
#include "Controller.h"
#include "Game Object.h"
#include "Event Manager.h"
#include "Sprite.h"
extern FrameRateController *gpFrc;
extern EventManager *gpEventManager;
UpDown::UpDown() : Component(UPDOWN)
{
	mGoingUp = true;
	mTimer = 0;
	mTimerLimit = 1000;
	isStart = true;
}

UpDown::~UpDown()
{
}

void UpDown::Update() {
	Sprite * pSp = static_cast<Sprite *> (mpOwner->GetComponent(SPRITE)); 
	if (mpOwner->objType == ZOMBIE2) {
		if (pSp->animationData->textureOffSetX < 0.29)
			return;
	}
	if (isStart) {
		mTimer += gpFrc->GetFrameTime();
		if (mTimer > mTimerLimit) {
			mGoingUp = !mGoingUp;
			mTimer = 0;
		}
		Body * pBody = static_cast<Body *> (mpOwner->GetComponent(BODY));
		Transform * pTr = static_cast<Transform *> (mpOwner->GetComponent(TRANSFORM));
		if (mGoingUp)
			pBody->mPosY += mSpeed;
		else
			pBody->mPosY -= mSpeed;
	}
}

void UpDown::Serialize(GenericObject<false, Value::ValueType> t) {
	mGoingUp = (bool)t["goingUp"].GetBool();
	
	mTimerLimit = t["timerLimit"].GetInt();
	mSpeed = t["speed"].GetFloat();
	//gpEventManager->Subscribe(PLAYER_HIT, this->mpOwner);
	if (t.HasMember("start")) {
		isStart = t["start"].GetBool();
	}
}

void UpDown::HandleEvent(Event * pEvent) {
	/*if (PLAYER_HIT == pEvent->mType) {
		Transform * pTr = static_cast<Transform *> (mpOwner->GetComponent(TRANSFORM));
		if (nullptr != pTr) {
			pTr->mPosX += 50;
		}
	}*/

	if (LIFT3ON == pEvent->mType && mpOwner->objType == LIFT3) {
		isStart = true;
	}
}

Component * UpDown::VirtuConst() {
	return new UpDown(*this);
}