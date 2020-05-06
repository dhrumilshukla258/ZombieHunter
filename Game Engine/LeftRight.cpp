/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: LeftRight.cpp
Purpose: Implements LeftRight component of GameObject
Language: C++
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: CS529_dhrumilshukla_GameEngine1.0
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Creation date: 10/15/2018
- End Header --------------------------------------------------------*/

#include "LeftRight.h"
#include "Frame Rate Controller.h"
#include "Body.h"
#include "Sprite.h"
#include "Transform.h"
extern FrameRateController *gpFrc;

LeftRight::LeftRight() : Component(LEFTRIGHT)
{
	mGoingRight = true;
	mTimer = 0;
	mTimerLimit = 1000;
}


LeftRight::~LeftRight()
{
}

void LeftRight::Update() {
	Body * pBody = static_cast<Body *> (mpOwner->GetComponent(BODY));
	Sprite * pSp = static_cast<Sprite *> (mpOwner->GetComponent(SPRITE));

	if (mpOwner->objType == ZOMBIERUN) {
		if ( (pSp->animationData == pSp->mapAnimationData["run"] && 
			 (pSp->animationData->textureOffSetX < 0.7 &&
			  pSp->animationData->textureOffSetY == 0.5) )|| (pSp->animationData == pSp->mapAnimationData["dead"]))
			return;		
	}
	
	mTimer += gpFrc->GetFrameTime();
	if (mTimer > mTimerLimit) {	
		mGoingRight = !mGoingRight;
		mTimer = 0;
	}
	
	if (mGoingRight) {
		pSp->isFlip = false;
		pBody->mPosX += mSpeed;
	}
	else {
		pSp->isFlip = true;
		pBody->mPosX -= mSpeed;
	}
}

void LeftRight::Serialize(GenericObject<false, Value::ValueType> t) {
	mGoingRight = (bool)t["goingRight"].GetBool();
	mTimerLimit = t["timerLimit"].GetInt();
	mSpeed = t["speed"].GetFloat();

	//gpEventManager->Subscribe(PLAYER_HIT, this->mpOwner);
}

void LeftRight::HandleEvent(Event * pEvent)
{
}

Component * LeftRight::VirtuConst() {
	return new LeftRight(*this);
}