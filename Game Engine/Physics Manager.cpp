/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Physics Manager.cpp
Purpose: Manages how objects should respond on collision
Language: C++
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: CS529_dhrumilshukla_GameEngine1.0
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Creation date: 10/15/2018
- End Header --------------------------------------------------------*/

#include "Physics Manager.h"
#include "Game Object Manager.h"
#include "Collision Manager.h"
#include "Body.h"
#include <iostream>
#include "Object Factory.h"
#include "Controller.h"
#include <unordered_map>
#include "Transform.h"
#include "UpDown.h"
#include "Render Manager.h"
#include "Sprite.h"

extern bool isGameStopped;

extern Game_Object_Manager * gpGameObjectManager;
extern CollisionManager * gpCollisionManager;
extern ObjectFactory * gpObjectFactory;
extern RenderManager * gpRenderManager;
extern EventManager * gpEventManager;
extern bool appIsRunning;
extern bool isMenu;
bool isFloor1Created = false;
float fT;

PhysicsManager::PhysicsManager()
{
}

PhysicsManager::~PhysicsManager()
{
}

void PhysicsManager::Update(float frameTime) {
	fT = frameTime;
	ReUse(gpGameObjectManager->mGameObject);
	//Super Advanced Physics here

	for (auto c : gpCollisionManager->mContacts) {

		//Bullet Hit Zombie1 or Zombie2
		if ( (c->mpBodies[0]->mpOwner->objType == BULLET && c->mpBodies[1]->mpOwner->objType == ZOMBIE1) ||
			 (c->mpBodies[0]->mpOwner->objType == BULLET && c->mpBodies[1]->mpOwner->objType == ZOMBIE2) || 
			 (c->mpBodies[0]->mpOwner->objType == BULLET && c->mpBodies[1]->mpOwner->objType == ZOMBIERUN) ) {
			BulletHitZombie * pEvent = new BulletHitZombie();
			c->mpBodies[0]->HandleEvent(pEvent);
			c->mpBodies[1]->HandleEvent(pEvent);

			gpObjectFactory->DeleteGameObject(c->mpBodies[0]->mpOwner);
		}
		else if ( (c->mpBodies[0]->mpOwner->objType == ZOMBIE1 && c->mpBodies[1]->mpOwner->objType == BULLET) ||
				  (c->mpBodies[0]->mpOwner->objType == ZOMBIE2 && c->mpBodies[1]->mpOwner->objType == BULLET) ||
			      (c->mpBodies[0]->mpOwner->objType == ZOMBIERUN && c->mpBodies[1]->mpOwner->objType == BULLET) ){
			BulletHitZombie * pEvent = new BulletHitZombie();
			c->mpBodies[0]->HandleEvent(pEvent);
			c->mpBodies[1]->HandleEvent(pEvent);

			gpObjectFactory->DeleteGameObject(c->mpBodies[1]->mpOwner);

		}

		//Bullet Destory
		else if (c->mpBodies[0]->mpOwner->objType == BULLET && c->mpBodies[1]->mpOwner->objType == FLOOR) {
			gpObjectFactory->DeleteGameObject(c->mpBodies[0]->mpOwner);
		}
		else if (c->mpBodies[0]->mpOwner->objType == FLOOR && c->mpBodies[1]->mpOwner->objType == BULLET) {
			gpObjectFactory->DeleteGameObject(c->mpBodies[1]->mpOwner);
		}

		//Crate and Player
		else if ( (c->mpBodies[0]->mpOwner->objType == PLAYER && c->mpBodies[1]->mpOwner->objType == CRATE) ||
				  (c->mpBodies[0]->mpOwner->objType == CRATE && c->mpBodies[1]->mpOwner->objType == PLAYER) ) {
			c->ApplyImpulse();
			c->PositionCorrection();
		}


		else if ((c->mpBodies[0]->mpOwner->objType == PLAYER && c->mpBodies[1]->mpOwner->objType == ZOMBIE1) ||
				 (c->mpBodies[0]->mpOwner->objType == PLAYER && c->mpBodies[1]->mpOwner->objType == ZOMBIE2) ||
				 (c->mpBodies[0]->mpOwner->objType == PLAYER && c->mpBodies[1]->mpOwner->objType == ZOMBIERUN))
		{ 
			c->ApplyImpulse();
			c->PositionCorrection();
			c->mpBodies[0]->mpOwner->isHit = true;
			c->mpBodies[0]->health -= 1;
		}
		else if ((c->mpBodies[1]->mpOwner->objType == PLAYER && c->mpBodies[0]->mpOwner->objType == ZOMBIE1) ||
				 (c->mpBodies[1]->mpOwner->objType == PLAYER && c->mpBodies[0]->mpOwner->objType == ZOMBIE2) ||
				 (c->mpBodies[1]->mpOwner->objType == PLAYER && c->mpBodies[0]->mpOwner->objType == ZOMBIERUN))
		{
			c->ApplyImpulse();
			c->PositionCorrection();
			c->mpBodies[1]->mpOwner->isHit = true;
			c->mpBodies[1]->health -= 1;
		}

		//Floor and Lift Collisio between Zombie1, Zombie2, ZombieRun and Player
		else if ( (c->mpBodies[0]->mpOwner->objType == FLOOR  && c->mpBodies[1]->mpOwner->objType == PLAYER) || 
				  (c->mpBodies[0]->mpOwner->objType == PLAYER && c->mpBodies[1]->mpOwner->objType == FLOOR ) ) 
		{
			c->ApplyImpulse();
			c->PositionCorrection();
		}
		else if ( (c->mpBodies[0]->mpOwner->objType == LIFT && c->mpBodies[1]->mpOwner->objType == PLAYER) ||
				  (c->mpBodies[0]->mpOwner->objType == LIFT3 && c->mpBodies[1]->mpOwner->objType == PLAYER) )
		{
			//UpDown * pUp = static_cast<UpDown *> (c->mpBodies[0]->mpOwner->GetComponent(UPDOWN));
			c->ApplyImpulse();
			c->PositionCorrection();
		}
		else if ((c->mpBodies[0]->mpOwner->objType == PLAYER && c->mpBodies[1]->mpOwner->objType == LIFT) ||
			(c->mpBodies[0]->mpOwner->objType == PLAYER && c->mpBodies[1]->mpOwner->objType == LIFT3)) {
			//UpDown * pUp = static_cast<UpDown *> (c->mpBodies[1]->mpOwner->GetComponent(UPDOWN));
			c->ApplyImpulse();
			c->PositionCorrection();
		}


		//PressKey3
		else if ((c->mpBodies[0]->mpOwner->objType == PLAYER && c->mpBodies[1]->mpOwner->objType == PRESSKEY3) ) {
			//UpDown * pUp = static_cast<UpDown *> (c->mpBodies[1]->mpOwner->GetComponent(UPDOWN));
			c->ApplyImpulse();
			c->PositionCorrection();

			UpDown * pUd = static_cast<UpDown *>(c->mpBodies[1]->mpOwner->GetComponent(UPDOWN));
			pUd->isStart = true;
		}
		else if ((c->mpBodies[1]->mpOwner->objType == PLAYER && c->mpBodies[0]->mpOwner->objType == PRESSKEY3)) {
			c->ApplyImpulse();
			c->PositionCorrection();

			UpDown * pUd = static_cast<UpDown *>(c->mpBodies[0]->mpOwner->GetComponent(UPDOWN));
			pUd->isStart = true;
		}

		else if ((c->mpBodies[0]->mpOwner->objType == FLOOR && c->mpBodies[1]->mpOwner->objType == ZOMBIE1) ||
			(c->mpBodies[0]->mpOwner->objType == ZOMBIE1 && c->mpBodies[1]->mpOwner->objType == FLOOR)) {
			c->ApplyImpulse();
			c->PositionCorrection();
		}
		
		else if ((c->mpBodies[0]->mpOwner->objType == FLOOR && c->mpBodies[1]->mpOwner->objType == ZOMBIE2) ) {
			c->ApplyImpulse();
			c->PositionCorrection();

			Sprite * pSp = static_cast <Sprite *> (c->mpBodies[1]->mpOwner->GetComponent(SPRITE));
			Body * pBody = static_cast <Body *> (c->mpBodies[1]->mpOwner->GetComponent(BODY));
			if (pSp->animationData->textureOffSetX > 0.27 ) {
				if (c->mpBodies[1]->health > 0) {
					pBody->mTotalForceY = 4500.0f;
				}
			}
		}
		else if (c->mpBodies[0]->mpOwner->objType == ZOMBIE2 && c->mpBodies[1]->mpOwner->objType == FLOOR) {
			c->ApplyImpulse();
			c->PositionCorrection();
			Sprite * pSp = static_cast <Sprite *> (c->mpBodies[0]->mpOwner->GetComponent(SPRITE));
			Body * pBody = static_cast <Body *> (c->mpBodies[0]->mpOwner->GetComponent(BODY));
			if (pSp->animationData->textureOffSetX > 0.27) {
				if (c->mpBodies[0]->health > 0) {
					pBody->mTotalForceY = 4500.0f;
				}
			}
		}
		else if ((c->mpBodies[0]->mpOwner->objType == FLOOR && c->mpBodies[1]->mpOwner->objType == ZOMBIERUN) ||
			(c->mpBodies[0]->mpOwner->objType == ZOMBIERUN && c->mpBodies[1]->mpOwner->objType == FLOOR)) {
			c->ApplyImpulse();
			c->PositionCorrection();
		}
		
		//Circle Reflection with Line Segment done in Collision Manager
		//Circle Deletion
		else if (c->mpBodies[0]->mpOwner->objType == CIRCLE && c->mpBodies[1]->mpOwner->objType == FLOOR) {
			gpObjectFactory->DeleteGameObject(c->mpBodies[0]->mpOwner);
		}
		else if (c->mpBodies[1]->mpOwner->objType == CIRCLE && c->mpBodies[0]->mpOwner->objType == FLOOR) {
			gpObjectFactory->DeleteGameObject(c->mpBodies[1]->mpOwner);
		}
		else if ( (c->mpBodies[0]->mpOwner->objType == CIRCLE && c->mpBodies[1]->mpOwner->objType == ZOMBIE1) ||
				  (c->mpBodies[0]->mpOwner->objType == CIRCLE && c->mpBodies[1]->mpOwner->objType == ZOMBIE2) ||
				  (c->mpBodies[0]->mpOwner->objType == CIRCLE && c->mpBodies[1]->mpOwner->objType == ZOMBIERUN)) {
			gpObjectFactory->DeleteGameObject(c->mpBodies[0]->mpOwner);
		}
		else if ((c->mpBodies[1]->mpOwner->objType == CIRCLE && c->mpBodies[0]->mpOwner->objType == ZOMBIE1) ||
			(c->mpBodies[1]->mpOwner->objType == CIRCLE && c->mpBodies[0]->mpOwner->objType == ZOMBIE2) ||
			(c->mpBodies[1]->mpOwner->objType == CIRCLE && c->mpBodies[0]->mpOwner->objType == ZOMBIERUN)) {
			gpObjectFactory->DeleteGameObject(c->mpBodies[1]->mpOwner);
		}

		else if ((c->mpBodies[1]->mpOwner->objType == CIRCLE && c->mpBodies[0]->mpOwner->objType == CRATE)) {
		if (c->mpBodies[1]->mVelX > 0) { c->mpBodies[0]->mPosX += 10.0f; }
		else { c->mpBodies[0]->mPosX -= 10.0f; }
		gpObjectFactory->DeleteGameObject(c->mpBodies[1]->mpOwner);
		}
		else if ((c->mpBodies[0]->mpOwner->objType == CIRCLE && c->mpBodies[1]->mpOwner->objType == CRATE)) {
		if (c->mpBodies[0]->mVelX > 0) { c->mpBodies[1]->mPosX += 10.0f; }
		else { c->mpBodies[1]->mPosX -= 10.0f; }
		gpObjectFactory->DeleteGameObject(c->mpBodies[0]->mpOwner);
		}

		//Knife
		else if (c->mpBodies[0]->mpOwner->objType == KNIFE && c->mpBodies[1]->mpOwner->objType == PLAYER) {
		c->mpBodies[1]->mpOwner->isHit = true;
		c->mpBodies[1]->health -= 2;
		gpObjectFactory->DeleteGameObject(c->mpBodies[0]->mpOwner);
		}
		else if (c->mpBodies[0]->mpOwner->objType == PLAYER && c->mpBodies[1]->mpOwner->objType == KNIFE) {
		c->mpBodies[0]->mpOwner->isHit = true;
		c->mpBodies[0]->health -= 2;
		gpObjectFactory->DeleteGameObject(c->mpBodies[1]->mpOwner);
		}

		//Bring Floor
		else if ((c->mpBodies[0]->mpOwner->objType == PRESSKEY1 ||
		c->mpBodies[0]->mpOwner->objType == PRESSKEY2 ||
		c->mpBodies[0]->mpOwner->objType == PRESSKEY3) &&
		c->mpBodies[1]->mpOwner->objType == CIRCLE) {
		CollideEvent * pCevent = new CollideEvent();
		c->mpBodies[0]->HandleEvent(pCevent);
		gpObjectFactory->DeleteGameObject(c->mpBodies[1]->mpOwner);
		}
		else if ((c->mpBodies[1]->mpOwner->objType == PRESSKEY1 ||
		c->mpBodies[1]->mpOwner->objType == PRESSKEY2 ||
		c->mpBodies[1]->mpOwner->objType == PRESSKEY3) &&
		c->mpBodies[0]->mpOwner->objType == CIRCLE) {
		CollideEvent * pCevent = new CollideEvent();
		gpObjectFactory->DeleteGameObject(c->mpBodies[0]->mpOwner);
		c->mpBodies[1]->HandleEvent(pCevent);
		}

		//Circle hit Grenade
		else if (c->mpBodies[0]->mpOwner->objType == CIRCLE && c->mpBodies[1]->mpOwner->objType == GRENADE)
		{
			Sprite * pSp = static_cast<Sprite *> (c->mpBodies[1]->mpOwner->GetComponent(SPRITE));

			if (!pSp->isCollide) {
				pSp->isCollide = true;
				ShapeAABB * sA = static_cast<ShapeAABB *> (c->mpBodies[1]->mpShape);

				sA->mBottom = 400;
				sA->mTop = 400;
				sA->mLeft = 300;
				sA->mRight = 300;

				Transform * pTr = static_cast<Transform *> (c->mpBodies[1]->mpOwner->GetComponent(TRANSFORM));
				pTr->mScaleX = 300;
				pTr->mScaleY = 300;


				pSp->animationData = pSp->mapAnimationData["blast"];

				pSp->animationData->textureOffSetX = 0.0f;
				pSp->animationData->textureOffSetY = 1.0f - pSp->animationData->mulTextureY;
				pSp->animationData->tempForLoopingPurpose = 0;


				pSp->texture = pSp->mapTexture["blast"];
			}
		}
		
		else if (c->mpBodies[0]->mpOwner->objType == GRENADE && c->mpBodies[1]->mpOwner->objType == CIRCLE)
		{
			Sprite * pSp = static_cast<Sprite *> (c->mpBodies[0]->mpOwner->GetComponent(SPRITE));
			
			if (!pSp->isCollide) {
				pSp->isCollide = true;
				ShapeAABB * sA = static_cast<ShapeAABB *> (c->mpBodies[1]->mpShape);

				sA->mBottom = 400;
				sA->mTop = 400;
				sA->mLeft = 300;
				sA->mRight = 300;
				Transform * pTr = static_cast<Transform *> (c->mpBodies[0]->mpOwner->GetComponent(TRANSFORM));
				pTr->mScaleX = 300;
				pTr->mScaleY = 300;

				pSp->animationData = pSp->mapAnimationData["blast"];

				pSp->animationData->textureOffSetX = 0.0f;
				pSp->animationData->textureOffSetY = 1.0f - pSp->animationData->mulTextureY;
				pSp->animationData->tempForLoopingPurpose = 0;
				pSp->current_time = 0.0f;
				pSp->texture = pSp->mapTexture["blast"];
			}
		}


		//Grenade hit Zombie 
		else if ((c->mpBodies[0]->mpOwner->objType == ZOMBIE1 && c->mpBodies[1]->mpOwner->objType == GRENADE) ||
		(c->mpBodies[0]->mpOwner->objType == ZOMBIE2 && c->mpBodies[1]->mpOwner->objType == GRENADE) ||
		(c->mpBodies[0]->mpOwner->objType == ZOMBIERUN && c->mpBodies[1]->mpOwner->objType == GRENADE))
		{
			Sprite * pSp = static_cast<Sprite *> (c->mpBodies[1]->mpOwner->GetComponent(SPRITE));
			if (pSp->animationData == pSp->mapAnimationData["blast"]) {
				gpObjectFactory->DeleteGameObject(c->mpBodies[0]->mpOwner);
				//gpObjectFactory->DeleteGameObject(c->mpBodies[1]->mpOwner);
			}
		}
		
		else if ( (c->mpBodies[0]->mpOwner->objType == GRENADE && c->mpBodies[1]->mpOwner->objType == ZOMBIE1) ||
		(c->mpBodies[0]->mpOwner->objType == GRENADE && c->mpBodies[1]->mpOwner->objType == ZOMBIE2) ||
		(c->mpBodies[0]->mpOwner->objType == GRENADE && c->mpBodies[1]->mpOwner->objType == ZOMBIERUN))
		{
		Sprite * pSp = static_cast<Sprite *> (c->mpBodies[0]->mpOwner->GetComponent(SPRITE));
		if (pSp->animationData == pSp->mapAnimationData["blast"]) {
			//gpObjectFactory->DeleteGameObject(c->mpBodies[0]->mpOwner);
			gpObjectFactory->DeleteGameObject(c->mpBodies[1]->mpOwner);
		}
		}

	}
}

void PhysicsManager::MenuUpdate(float frameTime) {
	fT = frameTime;

	ReUse(gpGameObjectManager->mMenuObject);

	for (auto c : gpCollisionManager->mContacts) {
		/*std::cout << c->mpBodies[0]->mpOwner << "\t";
		std::cout << c->mpBodies[1]->mpOwner << "\n";*/

		if ( ((c->mpBodies[0]->mpOwner->objType == NEWBUTTON)   ||
			   (c->mpBodies[1]->mpOwner->objType == NEWBUTTON)) &&
			 ((c->mpBodies[0]->mpOwner->objType == MOUSECLICKINMENU) ||
			   (c->mpBodies[0]->mpOwner->objType == MOUSECLICKINMENU)) ) 
		{
			for (auto go : gpGameObjectManager->mMenuObject) {
				delete go;
			}
			gpGameObjectManager->mMenuObject.clear();
			gpObjectFactory->LoadLevel("TwoModes.json", isMenu);

			//std::cout << "A";
			break;
		}
		
		if (((c->mpBodies[0]->mpOwner->objType == CAMPAIGN) ||
			(c->mpBodies[1]->mpOwner->objType == CAMPAIGN)) &&
			((c->mpBodies[0]->mpOwner->objType == MOUSECLICKINMENU) ||
			(c->mpBodies[0]->mpOwner->objType == MOUSECLICKINMENU)))
		{
			gpRenderManager->survivalMode = false;
			isMenu = false;
			isGameStopped = false;

			for (auto go : gpGameObjectManager->mMenuObject) {
				delete go;
			}
			gpGameObjectManager->mMenuObject.clear();
			for (auto go : gpGameObjectManager->mGameObject) {
				delete go;
			}
			gpGameObjectManager->mGameObject.clear();
			gpObjectFactory->LoadLevel("CampaignMode.json", isMenu);
			
			//std::cout << "A";
			break;
		}

		if (((c->mpBodies[0]->mpOwner->objType == SURVIVAL) ||
			(c->mpBodies[1]->mpOwner->objType == SURVIVAL)) &&
			((c->mpBodies[0]->mpOwner->objType == MOUSECLICKINMENU) ||
			(c->mpBodies[0]->mpOwner->objType == MOUSECLICKINMENU)))
		{
			gpRenderManager->survivalMode = true;
			isMenu = false;
			isGameStopped = false;

			for (auto go : gpGameObjectManager->mMenuObject) {
				delete go;
			}
			gpGameObjectManager->mMenuObject.clear();
			for (auto go : gpGameObjectManager->mGameObject) {
				delete go;
			}
			gpGameObjectManager->mGameObject.clear();
			gpObjectFactory->LoadLevel("SurvivalMode.json", isMenu);

			//std::cout << "A";
			break;
		}


		if (((c->mpBodies[0]->mpOwner->objType == CONTROLBUTTONPAUSE) ||
			(c->mpBodies[1]->mpOwner->objType == CONTROLBUTTONPAUSE)) &&
			((c->mpBodies[0]->mpOwner->objType == MOUSECLICKINMENU) ||
			(c->mpBodies[0]->mpOwner->objType == MOUSECLICKINMENU)))
		{
			for (auto go : gpGameObjectManager->mMenuObject) {
				delete go;
			}
			gpGameObjectManager->mMenuObject.clear();
			
			gpObjectFactory->LoadLevel("ControlScreenPause.json", isMenu);

			//std::cout << "A";
			break;
		}

		if (((c->mpBodies[0]->mpOwner->objType == CONTROLBUTTONMAIN) ||
			(c->mpBodies[1]->mpOwner->objType == CONTROLBUTTONMAIN)) &&
			((c->mpBodies[0]->mpOwner->objType == MOUSECLICKINMENU) ||
			(c->mpBodies[1]->mpOwner->objType == MOUSECLICKINMENU)))
		{
			for (auto go : gpGameObjectManager->mMenuObject) {
				delete go;
			}
			gpGameObjectManager->mMenuObject.clear();
			
			gpObjectFactory->LoadLevel("ControlScreenMain.json", isMenu);

			break;
		}

		if (((c->mpBodies[0]->mpOwner->objType == BACKBUTTONCONTROLMAIN) ||
			(c->mpBodies[1]->mpOwner->objType == BACKBUTTONCONTROLMAIN)) &&
			((c->mpBodies[0]->mpOwner->objType == MOUSECLICKINMENU) ||
			(c->mpBodies[1]->mpOwner->objType == MOUSECLICKINMENU)))
		{
			for (auto go : gpGameObjectManager->mMenuObject) {
				delete go;
			}
			gpGameObjectManager->mMenuObject.clear();
			gpObjectFactory->LoadLevel("NewGameMenu.json", isMenu);
			gpObjectFactory->deadZombie = 0;
			break;
		}

		if (((c->mpBodies[0]->mpOwner->objType == BACKBUTTONCONTROLPAUSE) ||
			(c->mpBodies[1]->mpOwner->objType == BACKBUTTONCONTROLPAUSE)) &&
			((c->mpBodies[0]->mpOwner->objType == MOUSECLICKINMENU) ||
			(c->mpBodies[1]->mpOwner->objType == MOUSECLICKINMENU)))
		{
			for (auto go : gpGameObjectManager->mMenuObject) {
				delete go;
			}
			gpGameObjectManager->mMenuObject.clear();

			gpObjectFactory->LoadLevel("PauseMenu.json", isMenu);
			break;
		}

		if ( (c->mpBodies[0]->mpOwner->objType == QUITBUTTON) || 
			 (c->mpBodies[1]->mpOwner->objType == QUITBUTTON) &&
			((c->mpBodies[0]->mpOwner->objType == MOUSECLICKINMENU) ||
			(c->mpBodies[1]->mpOwner->objType == MOUSECLICKINMENU))) {
			appIsRunning = false;
		}

		if (((c->mpBodies[0]->mpOwner->objType == BACKBUTTON) ||
			(c->mpBodies[1]->mpOwner->objType == BACKBUTTON)) &&
			((c->mpBodies[0]->mpOwner->objType == MOUSECLICKINMENU) ||
			(c->mpBodies[1]->mpOwner->objType == MOUSECLICKINMENU)))
		{
			isGameStopped = false;
			isMenu = false;
			for (auto go : gpGameObjectManager->mMenuObject) {
				delete go;
			}
			gpGameObjectManager->mMenuObject.clear();
			break;
		}

		
		if (( (c->mpBodies[0]->mpOwner->objType == CREDITBUTTONMAIN) ||
			  (c->mpBodies[1]->mpOwner->objType == CREDITBUTTONMAIN) ) &&
			( (c->mpBodies[0]->mpOwner->objType == MOUSECLICKINMENU) ||
			  (c->mpBodies[1]->mpOwner->objType == MOUSECLICKINMENU) ) )
		{
			for (auto go : gpGameObjectManager->mMenuObject) {
				delete go;
			}
			gpGameObjectManager->mMenuObject.clear();

				gpObjectFactory->LoadLevel("CreditScreen.json", isMenu);

				break;
		}
	}
}

void PhysicsManager::ReUse(std::vector<Game_Object *> temp) {
	//Integrate all Body Component
	for (auto go : temp)
	{
		Body * pBody = static_cast<Body *>(go->GetComponent(BODY));
		if (nullptr != pBody)
		{
			pBody->Integrate(fT);
		}
	}

	//Reset previous Contacts
	gpCollisionManager->Reset();

	//Check for Intersections

	auto pObj1 = temp.begin();
	auto pObjLast = temp.end();
	for (; pObj1 != pObjLast; ++pObj1) {
		Body * pBody1 = static_cast<Body *>((*pObj1)->GetComponent(BODY));

		if (nullptr == pBody1)
			continue;

		for (auto pObj2 = pObj1 + 1; pObj2 != pObjLast; ++pObj2) {
			Body * pBody2 = static_cast<Body *>((*pObj2)->GetComponent(BODY));

			if ( nullptr == pBody2 || (*pObj1)->objType == (*pObj2)->objType ||
				( (*pObj1)->objType == PLAYER && (*pObj2)->objType == BULLET ) ||
				( (*pObj1)->objType == ZOMBIE2 && (*pObj2)->objType == KNIFE ) ||
				( (*pObj1)->objType == PLAYER && (*pObj2)->objType == CIRCLE ) ||
				( (*pObj1)->objType == NOCOLLISIONTHING) || (*pObj2)->objType == NOCOLLISIONTHING ) 
				continue;

			gpCollisionManager->CheckCollisionAndGenerateContact(pBody1->mpShape, pBody1->mPosX, pBody1->mPosY,
				pBody2->mpShape, pBody2->mPosX, pBody2->mPosY);
		}
	}

}