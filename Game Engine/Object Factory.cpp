/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Object Factory.cpp
Purpose: Parses JSON files to read objects and
		 manages deletion and creation of objects
Language: C++
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: CS529_dhrumilshukla_GameEngine1.0
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Creation date: 11/01/2018
- End Header --------------------------------------------------------*/

#include "Object Factory.h"

//Rapid JSON files
#include <fstream>
#include <sstream>

#include <stdio.h>
#include <string>
#include <cstring>
#include <ctime>
#include "Event Manager.h"
#include "Game Object Manager.h"
#include "Resource Manager.h"
#include "Render Manager.h"
#include "Game Object.h"
#include "Transform.h"
#include "Component.h"
#include "Body.h"
#include "UpDown.h"
#include "LeftRight.h"
#include "Text.h"
#include "Sprite.h"
using namespace rapidjson;
extern Game_Object_Manager *gpGameObjectManager;
extern ResourceManager *gpResourceManager;
extern EventManager * gpEventManager;
extern RenderManager * gpRenderManager;
void type(std::string, Game_Object *);

ObjectFactory::ObjectFactory(){
	liveZombie = 0;
	deadZombie = 0;
	bomb = 0;
	bombCreated = false;
	whichZombie = true;
	leftRightZombie = true;
}

ObjectFactory::~ObjectFactory()
{
	deleteObject.clear();

	for (auto go : runTimeObjects)
		delete go;	
	runTimeObjects.clear();

	for (auto go: objectList)
		delete go.second;
	objectList.clear();

}

void ObjectFactory::LoadLevel(const char * pFileName, bool isMenu)
{
	std::string fullPath = "..\\Resources\\Levels\\";
	fullPath += pFileName;

	Document doc = loadDocument(fullPath);
	if (doc.HasMember("LiveZombie")) {
		liveZombie = doc["LiveZombie"]["num"].GetInt();
	}
	if (doc.HasMember("BackGround")) {
		std::string imageFilePath = "..\\Resources\\images\\";
		imageFilePath += doc["BackGround"]["BgImage"].GetString();
		textureBackground = gpResourceManager->LoadTexture(imageFilePath);
		VAOBackground = gpResourceManager->LoadMesh("Square");
	}
	if (doc.HasMember("BackGroundBlur")) {
		std::string imageFilePath = "..\\Resources\\images\\";
		imageFilePath += doc["BackGroundBlur"]["BgImage"].GetString();
		textureBackgroundBlur = gpResourceManager->LoadTexture(imageFilePath);
		VAOBackgroundBlur = gpResourceManager->LoadMesh("Square");
	}
	if (doc.HasMember("ListOfGameObjects")) {
		int size = doc["ListOfGameObjects"]["Objs"].Size();
		for (int i = 0; i < size; ++i) {
			std::string objName = doc["ListOfGameObjects"]["Objs"][i].GetString();

			objectList[objName] = LoadObject(objName.c_str());
		}
	}

	//Now the parsing starts
	for (auto itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr) {
		std::string objectName = itr->name.GetString();		
		
		GenericObject<false, Value::ValueType> temp = itr->value.GetObject();
		std::string fileName;
		Game_Object * pGameObject = nullptr;


		//Read Filename
		if (temp.HasMember("File")) {
			fileName = std::string(temp["File"].GetString());
			pGameObject = LoadObject(fileName.c_str());
			if (isMenu)
				gpGameObjectManager->mMenuObject.push_back(pGameObject);
			else
				gpGameObjectManager->mGameObject.push_back(pGameObject);
		}
		
		if (temp.HasMember("Transform")) {
			//GenericObject<false, Value::ValueType> t = temp["Transform"].GetObject();
			Transform * pTr = static_cast<Transform *> (pGameObject->GetComponent(TRANSFORM));
			pTr->Serialize( temp["Transform"].GetObject() );
		}

		if (temp.HasMember("Body")) {
			//GenericObject<false, Value::ValueType> t = temp["Transform"].GetObject();
			Body * pBody = static_cast<Body *>(pGameObject->GetComponent(BODY));
			pBody->Serialize( temp["Body"].GetObject() );
			pBody->Initialize();
		}

		if (temp.HasMember("UpDown")) {
			UpDown * pUpDown = static_cast<UpDown *>(pGameObject->GetComponent(UPDOWN));
			pUpDown->Serialize(temp["UpDown"].GetObject());
		}
		
		if (temp.HasMember("Sprite")) {
			Sprite * pSp = static_cast<Sprite *>(pGameObject->GetComponent(SPRITE));
			pSp->Serialize(temp["Sprite"].GetObject());
		}

		if (temp.HasMember("LeftRight")) {
			LeftRight * pLeftRight = static_cast<LeftRight *>(pGameObject->GetComponent(LEFTRIGHT));
			pLeftRight->Serialize(temp["LeftRight"].GetObject());
		}

		if (temp.HasMember("Type")) {
			std::string objTypeString = temp["Type"].GetString();
			type(objTypeString, pGameObject);
		}

		if (temp.HasMember("Text")) {
			Text * pText = static_cast<Text *>(pGameObject->GetComponent(TEXT));
			pText->Serialize(temp["Text"].GetObject());
		}
	}

}

Game_Object * ObjectFactory::CreateRunTimeObject(std::string pFileName) {
	Game_Object * runTimeObj = new Game_Object;
	runTimeObj->objType = objectList[pFileName]->objType;

	for (auto compo : objectList[pFileName]->mpComponent)
	{
		Component * x = compo->VirtuConst();
		x->mpOwner = runTimeObj;
		runTimeObj->mpComponent.push_back(x);
	}

	runTimeObjects.push_back(runTimeObj);

	return runTimeObj;
}

void ObjectFactory::DeleteGameObject(Game_Object * gameObject) {
	std::srand((unsigned int)std::time(nullptr));
	if (gpRenderManager->survivalMode) {
		if (gameObject->objType == ZOMBIERUN) {
			bombCreated = true;
			++deadZombie;
			if (walkZombie) {
				walkZombie = !walkZombie;
				Game_Object * pNewGameObject = CreateRunTimeObject("Zombie1.json");

				Body * pNewBody = static_cast<Body *> (pNewGameObject->GetComponent(BODY));
				Transform * pNewTr = static_cast <Transform *>(pNewGameObject->GetComponent(TRANSFORM));
				Sprite * pNewSp = static_cast <Sprite *>(pNewGameObject->GetComponent(SPRITE));

				Body * pBody = static_cast<Body *> (gameObject->GetComponent(BODY));
				Transform * pTr = static_cast <Transform *>(gameObject->GetComponent(TRANSFORM));
				Sprite * pSp = static_cast <Sprite *>(gameObject->GetComponent(SPRITE));

				pNewBody->mPosX = (float)635 + rand() % 130;
				pNewBody->mPosY = 300;

				pNewTr->mPosX = pNewBody->mPosX;
				pNewTr->mPosY = 300;

				pNewTr->mScaleX = 220;
				pNewTr->mScaleY = 150;
			}
			else {
				walkZombie = !walkZombie;
				Game_Object * pNewGameObject = CreateRunTimeObject("Zombie1Walk.json");

				Body * pNewBody = static_cast<Body *> (pNewGameObject->GetComponent(BODY));
				Transform * pNewTr = static_cast <Transform *>(pNewGameObject->GetComponent(TRANSFORM));
				Sprite * pNewSp = static_cast <Sprite *>(pNewGameObject->GetComponent(SPRITE));

				Body * pBody = static_cast<Body *> (gameObject->GetComponent(BODY));
				Transform * pTr = static_cast <Transform *>(gameObject->GetComponent(TRANSFORM));
				Sprite * pSp = static_cast <Sprite *>(gameObject->GetComponent(SPRITE));

				pNewBody->mPosX = (float) 65 + rand() % 110;
				pNewBody->mPosY = 300;

				pNewTr->mPosX = pNewBody->mPosX;
				pNewTr->mPosY = 300;

				pNewTr->mScaleX = 220;
				pNewTr->mScaleY = 180;

				pNewSp->isFlip = false;
			}
		}
		if (gameObject->objType == ZOMBIE1) {
			bombCreated = true;
			++deadZombie;
			if (leftRightZombie) {
				leftRightZombie = !leftRightZombie;
				Game_Object * pNewGameObject = CreateRunTimeObject("Zombie2.json");

				Body * pNewBody = static_cast<Body *> (pNewGameObject->GetComponent(BODY));
				Transform * pNewTr = static_cast <Transform *>(pNewGameObject->GetComponent(TRANSFORM));
				Sprite * pNewSp = static_cast <Sprite *>(pNewGameObject->GetComponent(SPRITE));

				Body * pBody = static_cast<Body *> (gameObject->GetComponent(BODY));
				Transform * pTr = static_cast <Transform *>(gameObject->GetComponent(TRANSFORM));
				Sprite * pSp = static_cast <Sprite *>(gameObject->GetComponent(SPRITE));

				pNewBody->mPosX = (float)635 + rand()%130;
				pNewBody->mPosY = 515;

				pNewTr->mPosX = pNewBody->mPosX;
				pNewTr->mPosY = 515;

				pNewTr->mScaleX = 220;
				pNewTr->mScaleY = 180;

				pNewSp->isFlip = true;
			}
			else {
				leftRightZombie = !leftRightZombie;
				Game_Object * pNewGameObject = CreateRunTimeObject("Zombie2.json");

				Body * pNewBody = static_cast<Body *> (pNewGameObject->GetComponent(BODY));
				Transform * pNewTr = static_cast <Transform *>(pNewGameObject->GetComponent(TRANSFORM));
				Sprite * pNewSp = static_cast <Sprite *>(pNewGameObject->GetComponent(SPRITE));

				Body * pBody = static_cast<Body *> (gameObject->GetComponent(BODY));
				Transform * pTr = static_cast <Transform *>(gameObject->GetComponent(TRANSFORM));
				Sprite * pSp = static_cast <Sprite *>(gameObject->GetComponent(SPRITE));

				pNewBody->mPosX = (float)65 + rand()%110;
				pNewBody->mPosY = 515;

				pNewTr->mPosX = pNewBody->mPosX;
				pNewTr->mPosY = 515;

				pNewTr->mScaleX = 220;
				pNewTr->mScaleY = 180;

				pNewSp->isFlip = false;
			}
		}
		if (gameObject->objType == ZOMBIE2) {
			bombCreated = true;
			++deadZombie;
			if (whichZombie) {
				whichZombie = !whichZombie;
				Game_Object * pNewGameObject = CreateRunTimeObject("ZombieRun1.json");

				Body * pBody = static_cast<Body *> (pNewGameObject->GetComponent(BODY));
				Transform * pTr = static_cast <Transform *>(pNewGameObject->GetComponent(TRANSFORM));
				Sprite * pSp = static_cast <Sprite *>(pNewGameObject->GetComponent(SPRITE));
				LeftRight * pLeftRight = static_cast <LeftRight *>(pNewGameObject->GetComponent(LEFTRIGHT));

				pTr->mPosX = (float)400 - rand()%300 ;
				pTr->mPosY = 145;

				pTr->mScaleX = 220;
				pTr->mScaleY = 180;

				pBody->mPosX = pTr->mPosX;
				pBody->mPosY = 145;

				pLeftRight->mTimerLimit = 20000;
				pLeftRight->mGoingRight = true;
				pLeftRight->mSpeed = 2.0f;

				pSp->isFlip = false;
			}
			else {
				whichZombie = !whichZombie;
				Game_Object * pNewGameObject = CreateRunTimeObject("ZombieRun2.json");

				Body * pBody = static_cast<Body *> (pNewGameObject->GetComponent(BODY));
				Transform * pTr = static_cast <Transform *>(pNewGameObject->GetComponent(TRANSFORM));
				Sprite * pSp = static_cast <Sprite *>(pNewGameObject->GetComponent(SPRITE));
				LeftRight * pLeftRight = static_cast <LeftRight *>(pNewGameObject->GetComponent(LEFTRIGHT));

				pTr->mPosX = (float)400 + rand()%300;
				pTr->mPosY = 145;

				pTr->mScaleX = 220;
				pTr->mScaleY = 180;

				pBody->mPosX = pTr->mPosX;
				pBody->mPosY = 145;

				pLeftRight->mTimerLimit = 20000;
				pLeftRight->mGoingRight = false;
				pLeftRight->mSpeed = 2.0f;

				pSp->isFlip = true;
			}
		}
		if (deadZombie % 5 == 0 && bombCreated) {
			bombCreated = false;
			Game_Object * pNewGameObject = CreateRunTimeObject("Grenade.json");
			
			Body * pNewBody = static_cast<Body *> (pNewGameObject->GetComponent(BODY));
			Transform * pNewTr = static_cast <Transform *>(pNewGameObject->GetComponent(TRANSFORM));
			Sprite * pNewSp = static_cast <Sprite *>(pNewGameObject->GetComponent(SPRITE));

			if (bomb == 0) {
				pNewBody->mPosX = (float)(30 + rand() % 150);
				pNewBody->mPosY = 425.0f;

				pNewTr->mPosX = pNewBody->mPosX;
				pNewTr->mPosY = pNewBody->mPosY;
				++bomb;
			}
			else if (bomb == 1) {
				pNewBody->mPosX = (float)(30 + rand() % 150);
				pNewBody->mPosY = 220.0f;

				pNewTr->mPosX = pNewBody->mPosX;
				pNewTr->mPosY = pNewBody->mPosY;
				++bomb;
			}
			else if (bomb==2){
				pNewBody->mPosX = (float)(rand() % 700);
				pNewBody->mPosY = 70.0f;

				pNewTr->mPosX = pNewBody->mPosX;
				pNewTr->mPosY = pNewBody->mPosY;
				++bomb;
			}
			else if (bomb==3) {
				pNewBody->mPosX = (float)(630 + rand() % 150);
				pNewBody->mPosY = 275.0f;

				pNewTr->mPosX = pNewBody->mPosX;
				pNewTr->mPosY = pNewBody->mPosY;
				++bomb;
			}
			else {
				pNewBody->mPosX = (float)(630 + rand() % 150);
				pNewBody->mPosY = 475.0f;

				pNewTr->mPosX = pNewBody->mPosX;
				pNewTr->mPosY = pNewBody->mPosY;
				bomb = 0;
			}

			pNewTr->mScaleX = 30;
			pNewTr->mScaleY = 40;

		}
	}
	else {
		if (gameObject->objType == ZOMBIERUN || gameObject->objType == ZOMBIE1 || gameObject->objType == ZOMBIE2) { ++deadZombie; }
	}
	deleteObject.push_back(gameObject);
}

void ObjectFactory::Update() {
	//Creation of Object
	for (auto go : runTimeObjects) {
		gpGameObjectManager->mGameObject.push_back(go);
	}
	runTimeObjects.clear();

	//Deletion of Object
	for (auto go : deleteObject) {
		std::vector<Game_Object *>::iterator itr = std::find(gpGameObjectManager->mGameObject.begin(),
			gpGameObjectManager->mGameObject.end(),
			go);
		if (itr != gpGameObjectManager->mGameObject.end()) {
			gpEventManager->UnSubscirbe(ZOMBIEDEAD, go);
			gpGameObjectManager->mGameObject.erase(itr);
			delete go;
		}
	}
	deleteObject.clear();

}

Game_Object * ObjectFactory::LoadObject(const char * pFileName)
{
	Game_Object *pNewGameObject = nullptr;
	std::string fullPath = "..\\Resources\\Objects\\";
	fullPath += pFileName;

	Document doc = loadDocument(fullPath);

	pNewGameObject = new Game_Object();
	Component *pNewComponent = nullptr;

	//Now the parsing starts
	//pNewGameObject->objType = doc["Type"].GetString();
	if (doc.HasMember("Transform")) {
		pNewComponent = pNewGameObject->AddComponent(TRANSFORM);
		//GenericObject<false, Value::ValueType> t = doc["Transform"].GetObject();
		pNewComponent->Serialize(doc["Transform"].GetObject());
	}
	if (doc.HasMember("Sprite")) {
		pNewComponent = pNewGameObject->AddComponent(SPRITE);
		GenericObject<false, Value::ValueType> t = doc["Sprite"].GetObject();
		pNewComponent->Serialize(t);
	}
	if (doc.HasMember("Controller")) {
		pNewComponent = pNewGameObject->AddComponent(CONTROLLER);
	}
	if (doc.HasMember("UpDown")) {
		pNewComponent = pNewGameObject->AddComponent(UPDOWN);
		pNewComponent->Serialize(doc["UpDown"].GetObject());
	}
	if (doc.HasMember("Body")) {
		pNewComponent = pNewGameObject->AddComponent(BODY);
		pNewComponent->Serialize(doc["Body"].GetObject());
	}
	if (doc.HasMember("LeftRight")) {
		pNewComponent = pNewGameObject->AddComponent(LEFTRIGHT);
		pNewComponent->Serialize(doc["LeftRight"].GetObject());
	}
	if (doc.HasMember("Camera")) {
		pNewComponent = pNewGameObject->AddComponent(CAMERA);
		pNewComponent->Serialize(doc["Camera"].GetObject());
	}
	if (doc.HasMember("Text")) {
		pNewComponent = pNewGameObject->AddComponent(TEXT);
		pNewComponent->Serialize(doc["Text"].GetObject());
	}
	if (doc.HasMember("Type")) {
		std::string objTypeString = doc["Type"].GetString();
		type(objTypeString, pNewGameObject);
	}
	return pNewGameObject;
}

Document ObjectFactory::loadDocument(std::string fullPath) {
	std::ifstream objFile(fullPath);
	std::stringstream objStream;
	if (objFile) {
		objStream << objFile.rdbuf();
		objFile.close();
	}
	else { throw std::runtime_error("!!Unable to open JSON file"); }
	Document doc;
	if (doc.Parse(objStream.str().c_str()).HasParseError()) { throw std::invalid_argument("json parsor error"); }

	return doc;
}

void ObjectFactory::type(std::string objTypeString, Game_Object * pNewGameObject) {
	
	if (objTypeString == "PLAYER")
		pNewGameObject->objType = PLAYER;
	else if (objTypeString == "FLOOR")
		pNewGameObject->objType = FLOOR;
	else if (objTypeString == "CRATE")
		pNewGameObject->objType = CRATE;
	else if (objTypeString == "LIFT")
		pNewGameObject->objType = LIFT;
	else if (objTypeString == "LIFT3") {
		pNewGameObject->objType = LIFT3;
		gpEventManager->Subscribe(LIFT3ON, pNewGameObject);
	}
	else if (objTypeString == "ZOMBIE1") {
		Transform * pTr = static_cast<Transform *>(pNewGameObject->GetComponent(TRANSFORM));
		pTr->isOffset = 1;
		pNewGameObject->objType = ZOMBIE1;
	}
	else if (objTypeString == "ZOMBIE2") {
		Transform * pTr = static_cast<Transform *>(pNewGameObject->GetComponent(TRANSFORM));
		pTr->isOffset = 1;
		pNewGameObject->mTimer = 4;
		pNewGameObject->objType = ZOMBIE2;
	}
	else if (objTypeString == "ZOMBIERUN") {
		Transform * pTr = static_cast<Transform *>(pNewGameObject->GetComponent(TRANSFORM));
		pTr->isOffset = 1;
		pNewGameObject->objType = ZOMBIERUN;
	}
	else if (objTypeString == "BULLET")
		pNewGameObject->objType = BULLET;
	else if (objTypeString == "MISSILE")
		pNewGameObject->objType = MISSILE;
	else if (objTypeString == "KNIFE")
		pNewGameObject->objType = KNIFE;
	else if (objTypeString == "CIRCLE")
		pNewGameObject->objType = CIRCLE;
	else if (objTypeString == "LINESEGMENT")
		pNewGameObject->objType = LINESEGMENT;
	else if (objTypeString == "NOCOLLISIONTHING")
		pNewGameObject->objType = NOCOLLISIONTHING;
	else if (objTypeString == "PRESSKEY1")
		pNewGameObject->objType = PRESSKEY1;
	else if (objTypeString == "PRESSKEY2")
		pNewGameObject->objType = PRESSKEY2;
	else if (objTypeString == "PRESSKEY3")
		pNewGameObject->objType = PRESSKEY3;

	else if (objTypeString == "NEWBUTTON")
		pNewGameObject->objType = NEWBUTTON;
	else if (objTypeString == "QUITBUTTON")
		pNewGameObject->objType = QUITBUTTON;
	else if (objTypeString == "CONTROLBUTTONMAIN")
		pNewGameObject->objType = CONTROLBUTTONMAIN;
	else if (objTypeString == "CONTROLBUTTONPAUSE")
		pNewGameObject->objType = CONTROLBUTTONPAUSE;
	else if (objTypeString == "MOUSECLICKINMENU")
		pNewGameObject->objType = MOUSECLICKINMENU;
	else if (objTypeString == "BACKBUTTON")
		pNewGameObject->objType = BACKBUTTON;
	else if (objTypeString == "BACKBUTTONCONTROLMAIN")
		pNewGameObject->objType = BACKBUTTONCONTROLMAIN;
	else if (objTypeString == "BACKBUTTONCONTROLPAUSE")
		pNewGameObject->objType = BACKBUTTONCONTROLPAUSE;
	else if (objTypeString == "CREDITBUTTONMAIN")
		pNewGameObject->objType = CREDITBUTTONMAIN;
	else if (objTypeString == "CAMPAIGN")
		pNewGameObject->objType = CAMPAIGN;
	else if (objTypeString == "SURVIVAL")
		pNewGameObject->objType = SURVIVAL;
	else if (objTypeString == "GRENADE") {
		Transform * pTr = static_cast<Transform *>(pNewGameObject->GetComponent(TRANSFORM));
		pTr->isOffset = 1;
		pNewGameObject->objType = GRENADE;
	}
}

//void ObjectFactory::LoadModel(const char * pFileName) {
//	std::string fullPath = "..\\Resources\\Models\\";
//	fullPath += pFileName;
//	std::ifstream objFile(fullPath);
//	std::stringstream objStream;
//	if (objFile) {
//		objStream << objFile.rdbuf();
//		objFile.close();
//	}
//	else { throw std::runtime_error("!!Unable to open JSON file"); }
//	Document doc;
//	if (doc.Parse(objStream.str().c_str()).HasParseError()) { throw std::invalid_argument("json parsor error"); }
//
//	if (doc.HasMember["Vertices"] ) {
//		
//	}
//}