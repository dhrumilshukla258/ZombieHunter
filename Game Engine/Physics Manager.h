#pragma once
#include "Event Manager.h"
#include <vector>
#include "Game Object.h"

class CollideEvent : public Event
{
public:
	CollideEvent() : Event(COLLIDE) {}
	~CollideEvent() {}
};

class PlayerHitEvent : public Event {
public:
	PlayerHitEvent() : Event(PLAYER_HIT) {}
	~PlayerHitEvent() {}
};

class CreateBulletEvent : public Event {
	CreateBulletEvent() : Event(CREATE_BULLET) {}
	~CreateBulletEvent() {}
};

class ThrowKnifeEvent : public Event {
	ThrowKnifeEvent() : Event(THROW_KNIFE) {}
	~ThrowKnifeEvent() {}
};


class BulletHitZombie : public Event
{
public:
	BulletHitZombie() : Event(BULLETHITZOMBIE) {}
	~BulletHitZombie() {}
};

class ZombieDead : public Event {
public:
	ZombieDead() : Event(ZOMBIEDEAD) {}
	~ZombieDead() {}
};

class GameOver : public Event {
public:
	GameOver() : Event(GAMEOVEREVENT) {}
	~GameOver() {}
};

class Lift3On : public Event {
public:
	Lift3On() : Event(LIFT3ON){}
	~Lift3On() {}
};

class PhysicsManager
{
public:
	PhysicsManager();
	~PhysicsManager();
	void Update(float);
	void MenuUpdate(float);
	void ReUse(std::vector<Game_Object *>);
public:
	bool level2 = false;
};

