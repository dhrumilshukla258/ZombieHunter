#pragma once
#include <list>
#include <unordered_map>
class Game_Object;

enum EventType {
	COLLIDE,
	PLAYER_HIT,
	WALL_HIT,
	CREATE_BULLET,
	THROW_KNIFE,
	BULLETHITZOMBIE,
	ZOMBIEDEAD,
	GAMEOVEREVENT,
	LIFT3ON,
	DELETEGRENADE,
	NUM
};

class Event
{
public:
	Event(EventType );
	virtual ~Event() {}
public:
	EventType mType;
	float mTimer;
};

class EventManager {
public:
	EventManager();
	~EventManager();
	void BroadcastEvent(Event * pEvent);
	void AddTimedEvent(Event * pEvent);
	void Update(float FrameTime);
	void Subscribe(EventType Type, Game_Object * pgameObject);
	void BreadcastEventToSubscribe(Event *pEvent);
	void UnSubscirbe(EventType Type, Game_Object *pGameObjectOldSubscribe);
public:
	std::list<Event *>mTimedEvents;
	std::unordered_map<EventType, std::list<Game_Object *>> mSubscriptions;

};
