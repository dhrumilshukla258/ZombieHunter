/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Sprite.cpp
Purpose: Important part for the Rendering. Keeps track of textureOffset of
		 SpriteAnimation and also textureIds of GameObjects.
Language: C++
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: CS529_dhrumilshukla_GameEngine1.0
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Creation date: 10/15/2018
- End Header --------------------------------------------------------*/

#include <string>
#include "Sprite.h"
#include "Frame Rate Controller.h"
#include "Resource Manager.h"
#include "Transform.h"
#include "Game Object.h"
#include "Object Factory.h"
#include "Game Object Manager.h"

extern ResourceManager *gpResourceManager;
extern ObjectFactory * gpObjectFactory;
extern Game_Object_Manager *gpGameObjectManager;


extern FrameRateController * gpFrc;
using namespace rapidjson;


SpriteAnimationData::SpriteAnimationData() {}

SpriteAnimationData::~SpriteAnimationData() {}

Sprite::Sprite() : Component(SPRITE) {
	animationData = nullptr;
	texture = 0;
}

Sprite::~Sprite() {
	
//	for (auto x : mapAnimationData) {
//delete x.second;
//	}
//	mapAnimationData.clear();
//	mapTexture.clear();

}

void Sprite::Update() {
	
	SpriteAnimationData * ani = animationData;
	Transform * pTr = static_cast<Transform *> (mpOwner->GetComponent(TRANSFORM));
	
	if (ani != nullptr) {
		if (ani->tempForLoopingPurpose == ani->numberOfSpritesInAnImage) {
			pTr->isOffset = 0;
			ani->tempForLoopingPurpose = 0;
			if (mpOwner->objType == PLAYER) {
				pTr->isOffset = 1;
				animationData = mapAnimationData["idle"];
				texture = mapTexture["idle"];
			}
			if (mpOwner->objType == ZOMBIE1) {
				Body * pBody = static_cast<Body *> (mpOwner->GetComponent(BODY));
				if (pBody->health > 0) {
					if (isFlip) { pBody->mVelX = -5.0f; }
					else { pBody->mVelX = 5.0f; }
					pTr->isOffset = 1;
					animationData = mapAnimationData["walk"];
					texture = mapTexture["walk"];
				}
			}
			if (mpOwner->objType == ZOMBIERUN) {
				Body * pBody = static_cast<Body *> (mpOwner->GetComponent(BODY));
				if (pBody->health > 0) {
					pTr->isOffset = 1;
					animationData = mapAnimationData["run"];
					texture = mapTexture["run"];
				}
			}
			if (mpOwner->objType == ZOMBIE2) {
				Body * pBody = static_cast<Body *> (mpOwner->GetComponent(BODY));
				if (pBody->health > 0) {
					pTr->isOffset = 1;
					animationData = mapAnimationData["jump"];
					texture = mapTexture["jump"];
				}
			}
			if (mpOwner->objType == GRENADE) {
				pTr->isOffset = 1;
				animationData = mapAnimationData["grenade"];
				texture = mapTexture["grenade"];
			}
		}
		if (pTr->isOffset == 1 ) {
			if (current_time > ani->time_limit  && ani->isLoop) {
				current_time = 0.0f;
				ani->textureOffSetX += ani->mulTextureX;
				
				if (ani->textureOffSetX == 1.0f) {
					ani->textureOffSetX = 0.0f;
					ani->textureOffSetY -= ani->mulTextureY;
					if (ani->textureOffSetY < 0) {
						ani->textureOffSetY = 1.0f - ani->mulTextureY;
						Body * pBody = static_cast<Body *> (mpOwner->GetComponent(BODY));
						if (pBody->health <= 0) {
							ani->textureOffSetX = 1 - ani->mulTextureX;
							ani->textureOffSetY = 0.0;
						}
					}
				}
				++(ani->tempForLoopingPurpose);
			}
			else {
				current_time += gpFrc->GetFrameTime();
			}
		}
	}
}

void Sprite::Serialize(GenericObject<false, Value::ValueType> t) {

	if (t.HasMember("SpriteFiles")) {
		std::string textureFile = "..\\Resources\\images\\";
		std::string extra;

		if (t.HasMember("Folder")) {
			extra = t["Folder"].GetString();
		}
		
		extra += "\\";
		textureFile += extra;

		GenericObject<false, Value::ValueType> temp = t["SpriteFiles"].GetObject();
		
		if (temp.HasMember("walk")) {
			animationData = new SpriteAnimationData();
			animationData->numberOfSpritesInAnImage = temp["walk"]["numOfImages"].GetInt();
			textureFile += temp["walk"]["imageFile"].GetString();
			texture = gpResourceManager->LoadTexture(textureFile);

			animationData->mulTextureX = temp["walk"]["mulTextureX"].GetFloat();
			animationData->mulTextureY = temp["walk"]["mulTextureY"].GetFloat();
			animationData->time_limit = temp["walk"]["timerLimit"].GetFloat();

			animationData->textureOffSetX = 0.0f;
			animationData->textureOffSetY = 1.0f - animationData->mulTextureY;
			animationData->tempForLoopingPurpose = 0;

			mapTexture["walk"] = texture;
			mapAnimationData["walk"] = animationData;
		}

		textureFile = "..\\Resources\\images\\";
		textureFile += extra;
		if (temp.HasMember("attackbullet")) {
			animationData = new SpriteAnimationData();
			animationData->numberOfSpritesInAnImage = temp["attackbullet"]["numOfImages"].GetInt();
			textureFile += temp["attackbullet"]["imageFile"].GetString();
			texture = gpResourceManager->LoadTexture(textureFile);

			animationData->mulTextureX = temp["attackbullet"]["mulTextureX"].GetFloat();
			animationData->mulTextureY = temp["attackbullet"]["mulTextureY"].GetFloat();
			animationData->time_limit = temp["attackbullet"]["timerLimit"].GetFloat();

			animationData->textureOffSetX = 0.0f;
			animationData->textureOffSetY = 1.0f - animationData->mulTextureY;
			animationData->tempForLoopingPurpose = 0;

			mapTexture["attackbullet"] = texture;
			mapAnimationData["attackbullet"] = animationData;
		}

		textureFile = "..\\Resources\\images\\";
		textureFile += extra;
		if (temp.HasMember("attackcircle")) {
			animationData = new SpriteAnimationData();
			animationData->numberOfSpritesInAnImage = temp["attackcircle"]["numOfImages"].GetInt();
			textureFile += temp["attackcircle"]["imageFile"].GetString();
			texture = gpResourceManager->LoadTexture(textureFile);

			animationData->mulTextureX = temp["attackcircle"]["mulTextureX"].GetFloat();
			animationData->mulTextureY = temp["attackcircle"]["mulTextureY"].GetFloat();
			animationData->time_limit = temp["attackcircle"]["timerLimit"].GetFloat();

			animationData->textureOffSetX = 0.0f;
			animationData->textureOffSetY = 1.0f - animationData->mulTextureY;
			animationData->tempForLoopingPurpose = 0;
			if (temp["attackcircle"].GetObject().HasMember("isLoop")) {
				animationData->isLoop = temp["attackcircle"]["isLoop"].GetBool();
			}

			mapTexture["attackcircle"] = texture;
			mapAnimationData["attackcircle"] = animationData;
		}

		textureFile = "..\\Resources\\images\\";
		textureFile += extra;
		if (temp.HasMember("idle")) {
			animationData = new SpriteAnimationData();
			animationData->numberOfSpritesInAnImage = temp["idle"]["numOfImages"].GetInt();
			textureFile += temp["idle"]["imageFile"].GetString();
			texture = gpResourceManager->LoadTexture(textureFile);

			animationData->mulTextureX = temp["idle"]["mulTextureX"].GetFloat();
			animationData->mulTextureY = temp["idle"]["mulTextureY"].GetFloat();
			animationData->time_limit = temp["idle"]["timerLimit"].GetFloat();

			animationData->textureOffSetX = 0.0f;
			animationData->textureOffSetY = 1.0f - animationData->mulTextureY;
			animationData->tempForLoopingPurpose = 0;

			mapTexture["idle"] = texture;
			mapAnimationData["idle"] = animationData;
		}

		textureFile = "..\\Resources\\images\\";
		textureFile += extra;
		if (temp.HasMember("dead")) {
			animationData = new SpriteAnimationData();
			animationData->numberOfSpritesInAnImage = temp["dead"]["numOfImages"].GetInt();
			textureFile += temp["dead"]["imageFile"].GetString();
			texture = gpResourceManager->LoadTexture(textureFile);

			animationData->mulTextureX = temp["dead"]["mulTextureX"].GetFloat();
			animationData->mulTextureY = temp["dead"]["mulTextureY"].GetFloat();
			animationData->time_limit = temp["dead"]["timerLimit"].GetFloat();

			animationData->textureOffSetX = 0.0f;
			animationData->textureOffSetY = 1.0f - animationData->mulTextureY;
			animationData->tempForLoopingPurpose = 0;

			mapTexture["dead"] = texture;
			mapAnimationData["dead"] = animationData;
		}

		textureFile = "..\\Resources\\images\\";
		textureFile += extra;
		if (temp.HasMember("hurt")) {
			animationData = new SpriteAnimationData();
			animationData->numberOfSpritesInAnImage = temp["hurt"]["numOfImages"].GetInt();
			textureFile += temp["hurt"]["imageFile"].GetString();
			texture = gpResourceManager->LoadTexture(textureFile);

			animationData->mulTextureX = temp["hurt"]["mulTextureX"].GetFloat();
			animationData->mulTextureY = temp["hurt"]["mulTextureY"].GetFloat();
			animationData->time_limit = temp["hurt"]["timerLimit"].GetFloat();

			animationData->textureOffSetX = 0.0f;
			animationData->textureOffSetY = 1.0f - animationData->mulTextureY;
			animationData->tempForLoopingPurpose = 0;

			mapTexture["hurt"] = texture;
			mapAnimationData["hurt"] = animationData;
		}

		textureFile = "..\\Resources\\images\\";
		textureFile += extra;
		if (temp.HasMember("run")) {
			animationData = new SpriteAnimationData();
			animationData->numberOfSpritesInAnImage = temp["run"]["numOfImages"].GetInt();
			textureFile += temp["run"]["imageFile"].GetString();
			texture = gpResourceManager->LoadTexture(textureFile);

			animationData->mulTextureX = temp["run"]["mulTextureX"].GetFloat();
			animationData->mulTextureY = temp["run"]["mulTextureY"].GetFloat();
			animationData->time_limit = temp["run"]["timerLimit"].GetFloat();

			animationData->textureOffSetX = 0.0f;
			animationData->textureOffSetY = 1.0f - animationData->mulTextureY;
			animationData->tempForLoopingPurpose = 0;

			mapTexture["run"] = texture;
			mapAnimationData["run"] = animationData;
		}

		textureFile = "..\\Resources\\images\\";
		textureFile += extra;
		if (temp.HasMember("jump")) {
			animationData = new SpriteAnimationData();
			animationData->numberOfSpritesInAnImage = temp["jump"]["numOfImages"].GetInt();
			textureFile += temp["jump"]["imageFile"].GetString();
			texture = gpResourceManager->LoadTexture(textureFile);

			animationData->mulTextureX = temp["jump"]["mulTextureX"].GetFloat();
			animationData->mulTextureY = temp["jump"]["mulTextureY"].GetFloat();
			animationData->time_limit = temp["jump"]["timerLimit"].GetFloat();

			animationData->textureOffSetX = 0.0f;
			animationData->textureOffSetY = 1.0f - animationData->mulTextureY;
			animationData->tempForLoopingPurpose = 0;

			mapTexture["jump"] = texture;
			mapAnimationData["jump"] = animationData;
		}

		textureFile = "..\\Resources\\images\\";
		if (temp.HasMember("blast")) {
			animationData = new SpriteAnimationData();
			animationData->numberOfSpritesInAnImage = temp["blast"]["numOfImages"].GetInt();
			textureFile += temp["blast"]["imageFile"].GetString();
			texture = gpResourceManager->LoadTexture(textureFile);

			animationData->mulTextureX = temp["blast"]["mulTextureX"].GetFloat();
			animationData->mulTextureY = temp["blast"]["mulTextureY"].GetFloat();
			animationData->time_limit = temp["blast"]["timerLimit"].GetFloat();

			animationData->textureOffSetX = 0.0f;
			animationData->textureOffSetY = 1.0f - animationData->mulTextureY;
			animationData->tempForLoopingPurpose = 0;

			mapTexture["blast"] = texture;
			mapAnimationData["blast"] = animationData;
		}

		textureFile = "..\\Resources\\images\\";
		if (temp.HasMember("grenade")) {
			animationData = new SpriteAnimationData();
			animationData->numberOfSpritesInAnImage = temp["grenade"]["numOfImages"].GetInt();
			textureFile += temp["grenade"]["imageFile"].GetString();
			texture = gpResourceManager->LoadTexture(textureFile);

			animationData->mulTextureX = temp["grenade"]["mulTextureX"].GetFloat();
			animationData->mulTextureY = temp["grenade"]["mulTextureY"].GetFloat();
			animationData->time_limit = temp["grenade"]["timerLimit"].GetFloat();

			animationData->textureOffSetX = 0.0f;
			animationData->textureOffSetY = 1.0f - animationData->mulTextureY;
			animationData->tempForLoopingPurpose = 0;

			mapTexture["grenade"] = texture;
			mapAnimationData["grenade"] = animationData;
		}

		if (temp.HasMember("isFlip")) {
			isFlip = temp["isFlip"].GetBool();
		}
		/*
		if (spriteJsonFile.find("jump") != std::string::npos) {
			mapTexture["jump"] = texture;
			mapAnimationData["jump"] = animationData;
		}

		if (spriteJsonFile.find("attack_knife") != std::string::npos) {
			mapTexture["attack_knife"] = texture;
			mapAnimationData["attack_knife"] = animationData;
		}

		if (spriteJsonFile.find("idle") != std::string::npos) {
			mapTexture["idle"] = texture;
			mapAnimationData["idle"] = animationData;
		}

		if (spriteJsonFile.find("attack_gun") != std::string::npos) {
			mapTexture["attack_gun"] = texture;
			mapAnimationData["attack_gun"] = animationData;
		}

		if (spriteJsonFile.find("dead") != std::string::npos) {
			mapTexture["dead"] = texture;
			mapAnimationData["dead"] = animationData;
		}

		if (spriteJsonFile.find("attack_crawl") != std::string::npos) {
			mapTexture["attack_crawl"] = texture;
			mapAnimationData["attack_crawl"] = animationData;
		}
		*/
	}

	/*if (t.HasMember("AnotherBody")) {
		std::string bodyFile = t["AnotherBody"].GetString();
		mAnotherBody = static_cast<AnotherBody *>( gpObjectFactory->LoadObject(bodyFile.c_str()) );
		gpGameObjectManager->mGameObject.push_back(mAnotherBody);
	}*/
	if (t.HasMember("Texture")) {
		int size = t["Texture"].Size();

		for (int i = 0; i < size; ++i) {
			std::string textureFile = "..\\Resources\\images\\";
			textureFile += t["Texture"][i].GetString();
			texture = gpResourceManager->LoadTexture(textureFile);
		}
	}
	if (t.HasMember("Shape")) {
		std::string shape = t["Shape"].GetString();
		VAO = gpResourceManager->LoadMesh(shape);
	}
}

Component * Sprite::VirtuConst() {
	Sprite * pSp = new Sprite(*this);
	for (auto cp : pSp->mapAnimationData) {
		cp.second->textureOffSetX = 0.0f;
		cp.second->textureOffSetY = 1.0f - cp.second->mulTextureY;
		cp.second->tempForLoopingPurpose = 0;
	}
	pSp->current_time = 0.0f;
	return pSp;
}