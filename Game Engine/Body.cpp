/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Body.cpp
Purpose: Implements Body component of GameObject
Language: C++
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: CS529_dhrumilshukla_GameEngine1.0
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Creation date: 10/29/2018
- End Header --------------------------------------------------------*/

#include "Body.h"
#include "Transform.h"
#include "Sprite.h"
#include "Collision Manager.h"
#include <iostream>
#include "Frame Rate Controller.h"
#include "Physics Manager.h"
#include "Game Object Manager.h"
#include "Object Factory.h"
#include "Event Manager.h"

float Gravity = 150.0f;

#define BULLET_SPEED 100.0f;

extern ObjectFactory * gpObjectFactory;
extern Game_Object_Manager * gpGameObjectManager;
extern EventManager * gpEventManager;

extern bool isMenu;
extern bool isGameStopped;

Body::Body() : Component(BODY)
{
	mPosX = mPosY = 0;
	mPrevPosX = mPrevPosY = 0;
	mVelX = mVelY = 0;
	mAccX = mAccY = 0;
	mTotalForceX = mTotalForceY = 0;
	mMass = mInvMass = 0;
}

Body::~Body() {
}

void Body::Update() {}

void Body::Serialize(GenericObject<false, Value::ValueType> t) {
	mMass = t["mass"].GetFloat();

	if (0.0f != mMass) {
		mInvMass = 1.0f / mMass;
	}
	else {
		mInvMass = 0.0;
	}
	
	if (t.HasMember("Circle")) {
		mpShape = new ShapeCircle();
		mpShape->mpOwnerBody = this;
		ShapeCircle * pCircle = static_cast<ShapeCircle *> (mpShape);

		pCircle->mRadius = t["Circle"]["radius"].GetFloat();
	}
	if (t.HasMember("Line")) {
		mpShape = new ShapeLineSegment();
		mpShape->mpOwnerBody = this;
		
		Transform * pTr = static_cast<Transform *>(mpOwner->GetComponent(TRANSFORM));
		ShapeLineSegment * pLs = static_cast<ShapeLineSegment *>(mpShape);

		pLs->mLength = t["Line"]["length"].GetFloat();
		
		pLs->mPos1y = pTr->mPosY  + (pLs->mLength / 2.0f * sinf(pTr->mAngle) );
		pLs->mPos0y = pTr->mPosY  - (pLs->mLength / 2.0f * sinf(pTr->mAngle) );

		pLs->mPos1x = pTr->mPosX + ( pLs->mLength / 2.0f * cosf(pTr->mAngle) );
		pLs->mPos0x = pTr->mPosX - ( pLs->mLength / 2.0f * cosf(pTr->mAngle) );

		
		if (nullptr != pTr) {
			pLs->mNx = pLs->mPos1y - pLs->mPos0y;
			pLs->mNy = pLs->mPos0x - pLs->mPos1x;
			
			float d = sqrt( (pLs->mNx * pLs->mNx) + (pLs->mNy * pLs->mNy) );
			
			pLs->mNx /= d;
			pLs->mNy /= d;

			pLs->mNdot = ( pLs->mNx * pLs->mPos0x ) + ( pLs->mNy * pLs->mPos0y );
			int i;
			i = 0;

		}
		
	}
	if (t.HasMember("AABB")) {
		mpShape = new ShapeAABB();
		mpShape->mpOwnerBody = this;
		ShapeAABB * pAABB = static_cast<ShapeAABB *> (mpShape);

		pAABB->mTop = t["AABB"]["top"].GetFloat();
		pAABB->mBottom = t["AABB"]["bottom"].GetFloat();
		pAABB->mRight = t["AABB"]["left"].GetFloat();
		pAABB->mLeft = t["AABB"]["right"].GetFloat();
	}
	if (t.HasMember("mVelX") && t.HasMember("mVelY")) {
		mVelX = t["mVelX"].GetFloat();
		mVelY = t["mVelY"].GetFloat();
	}
}

void Body::Initialize() {
	Transform * pTr = static_cast<Transform *> (mpOwner->GetComponent(TRANSFORM));
	if (nullptr != pTr) {
		mPrevPosX = mPosX = pTr->mPosX;
		mPrevPosY = mPosY = pTr->mPosY;
	}
}

void Body::Integrate( float DeltaTime) {
	Transform * pTr = static_cast<Transform *> (mpOwner->GetComponent(TRANSFORM));
	Sprite * pSp = static_cast<Sprite *> (mpOwner->GetComponent(SPRITE));
	// Save Current Position
	
	mPrevPosX = mPosX;
	mPrevPosY = mPosY;

	// Compute Acceleration
	mTotalForceY -= Gravity;
	mAccX = mTotalForceX * mInvMass;
	mAccY = mTotalForceY * mInvMass;
	
	// Integrate the velocity
	mVelX = mAccX * DeltaTime + mVelX;
	mVelY = mAccY * DeltaTime + mVelY;
		
	if (mpOwner->objType == PLAYER) {
		if (mPosY < -100 || health < 0) {
			isGameStopped = true;
			isMenu = true;
			gpObjectFactory->deadZombie = 0;
			gpObjectFactory->LoadLevel("TryAgainMenu.json", true);
			return;
		}
		if (mPosY > 800.0f) {
			isGameStopped = true;
			isMenu = true;
			gpObjectFactory->deadZombie = 0;
			gpObjectFactory->LoadLevel("TheEndMenu.json", true);
			return;
		}
		mVelX = mVelX * .975f;
		mVelY = mVelY * .975f;
	}
	//Integrate the position
	mPosX = mVelX * DeltaTime + mPosX;
	mPosY = mVelY * DeltaTime + mPosY;
	
	//mPosY = mPosY < -1.00f ? -1.00f : mVelY * DeltaTime + mPosY;
	//if (mPosY == -1.00f) { mVelY = 0.0f; }
	//mPosY = mVelY * DeltaTime + mPosY;	
	//if (mPosY == 3.00f) { mVelY = 0.0f; }
	
	
	mTotalForceX = mTotalForceY = 0.0f;
	
	if (nullptr != pTr) {
		pTr->mPosX = mPosX;
		pTr->mPosY = mPosY;
	}
}

Component * Body::VirtuConst() {
	Body * temp = new Body(*this);
	temp->mpShape = this->mpShape->ShapeVirtual();
	temp->mpShape->mpOwnerBody = temp;
	return temp;
}

void Body::HandleEvent(Event * pEvent) {
	if (COLLIDE == pEvent->mType && mpOwner->objType == PRESSKEY1) {
		if (!isFloorCreated) {
			Game_Object * pGameObject = nullptr;
			pGameObject = gpObjectFactory->CreateRunTimeObject("Floor1.json");

			Transform * newPtr = static_cast<Transform *> (pGameObject->GetComponent(TRANSFORM));
			Body *newPbody = static_cast<Body *> (pGameObject->GetComponent(BODY));

			ShapeAABB * r = static_cast<ShapeAABB *>(newPbody->mpShape);
			r->mBottom = 300.0f;
			r->mTop = 300.0f;
			r->mLeft = 50.0f;
			r->mRight = 50.0f;
			newPtr->mPosX = 1450.0f;
			newPtr->mPosY = 25.0f;
			newPtr->mScaleX = 300.0f;
			newPtr->mScaleY = 50.0f;
			newPbody->mPosX = newPtr->mPosX;
			newPbody->mPosY = newPtr->mPosY;
			newPbody->mMass = 0.0f;
			isFloorCreated = !isFloorCreated;
		}
	}
	if (COLLIDE == pEvent->mType && mpOwner->objType == PRESSKEY2) {
		Lift3On * pL3 = new Lift3On();
		pL3->mTimer = 1.0f;
		gpEventManager->AddTimedEvent(pL3);
	}
	if (COLLIDE == pEvent->mType && mpOwner->objType == PRESSKEY3) {
		/*if (!isFloorCreated) {
			Game_Object * pGameObject = nullptr;
			pGameObject = gpObjectFactory->CreateRunTimeObject("Floor1.json");

			Transform * newPtr = static_cast<Transform *> (pGameObject->GetComponent(TRANSFORM));
			Body *newPbody = static_cast<Body *> (pGameObject->GetComponent(BODY));

			newPtr->mPosX = 1700.0f;
			newPtr->mPosY = 25.0f;
			newPtr->mScaleX = 800.0f;
			newPtr->mScaleY = 50.0f;
			newPbody->mPosX = newPtr->mPosX;
			newPbody->mPosY = newPtr->mPosY;
			newPbody->mMass = 0.0f;
			isFloorCreated = !isFloorCreated;
			std::cout << "Madarchod\n";
		}*/
	}

	if (BULLETHITZOMBIE == pEvent->mType && 
		(mpOwner->objType == ZOMBIE1 || mpOwner->objType == ZOMBIE2 || 
			mpOwner->objType == ZOMBIERUN)) 
	{
		Sprite * pSp = static_cast<Sprite *>(mpOwner->GetComponent(SPRITE));
		Transform * pTr = static_cast<Transform *>(mpOwner->GetComponent(TRANSFORM));
		health -= 25;
		if (health == 0 ) {
			pTr->isOffset = 1;
			pSp->animationData = pSp->mapAnimationData["dead"];
			pSp->texture = pSp->mapTexture["dead"];

			ZombieDead * zD = new ZombieDead();
			zD->mTimer = 5.0f;
			
			mVelX = 0;
			mVelY = 0;
			ShapeAABB * sha = static_cast<ShapeAABB *> (mpShape);
			float temp = sha->mLeft;
			sha->mLeft = sha->mBottom;
			sha->mRight = sha->mTop;
			sha->mTop = temp;
			sha->mBottom = temp;

			gpEventManager->AddTimedEvent(zD);
			gpEventManager->Subscribe(ZOMBIEDEAD, this->mpOwner);
			
			//gpObjectFactory->DeleteGameObject(mpOwner);
		}
		else if (health > 0){
			pTr->isOffset = 1;
			pSp->animationData = pSp->mapAnimationData["hurt"];
			pSp->texture = pSp->mapTexture["hurt"];
		}
	}

	if (ZOMBIEDEAD == pEvent->mType) {
		gpObjectFactory->DeleteGameObject(this->mpOwner);
	}
}