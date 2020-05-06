/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Event Manager.cpp
Purpose: Manager event and subscription for all objects
Language: C++
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: CS529_dhrumilshukla_GameEngine1.0
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Creation date: 11/09/2018
- End Header --------------------------------------------------------*/

#include "Event Manager.h"
#include "Game Object Manager.h"
#include "Body.h"

extern Game_Object_Manager * gpGameObjectManager;

Event::Event(EventType type)
{
	mType = type;
}

EventManager::EventManager(){
}

EventManager::~EventManager() {

}

void EventManager::BroadcastEvent(Event * pEvent) {
	for (auto go : gpGameObjectManager->mGameObject)
		go->HandleEvent(pEvent);
}

void EventManager::AddTimedEvent(Event * pEvent) {
	mTimedEvents.push_back(pEvent);
}

void EventManager::Update(float FrameTime) {
	//Loop Through all time events
	//Decrease timer by Frametime
	// < 0 ? -> fire event, Remove from list
	std::list <Event*>::iterator it, itTemp, itEnd;

	itEnd = mTimedEvents.end();
	it = mTimedEvents.begin();

	while ( it != mTimedEvents.end()) {
		Event * pEvent = *it;
		pEvent->mTimer -= FrameTime;
		if (pEvent->mTimer < 0.0f) {	
			BreadcastEventToSubscribe(pEvent);
			it = mTimedEvents.erase(it);
			delete pEvent;
		}
		else {
			++it;
		}
	}
}

void EventManager::Subscribe(EventType Type, Game_Object * pGameObjectNewSubscriber) {
	//std::list <Game_Object*> &istOfsubscribers = mSubscriptions[Type];
	auto &listOfsubscribers = mSubscriptions[Type];
	
	for (auto pGo : listOfsubscribers)
		if (pGo == pGameObjectNewSubscriber)
			return;

	listOfsubscribers.push_back(pGameObjectNewSubscriber);
}

void EventManager::BreadcastEventToSubscribe(Event * pEvent) {
	auto &istOfsubscribers = mSubscriptions[pEvent->mType];
	for (auto pGo : istOfsubscribers)
		pGo->HandleEvent(pEvent);
}

void EventManager::UnSubscirbe(EventType Type, Game_Object *pGameObjectOldSubscribe) {
	auto &listOfSubscribers = mSubscriptions[Type];

	auto it = std::find(listOfSubscribers.begin(), 
						listOfSubscribers.end(),
						pGameObjectOldSubscribe);

	if (it == listOfSubscribers.end())
		return;

	listOfSubscribers.erase(it);
}