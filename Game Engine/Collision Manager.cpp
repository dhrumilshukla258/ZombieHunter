/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Collision Manager.cpp
Purpose: Checks Collision between different shapes, Reflection, Impulse, Correctness
Language: C++
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: CS529_dhrumilshukla_GameEngine1.0
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Creation date: 11/20/2018
- End Header --------------------------------------------------------*/

#include "Collision Manager.h"
#include "Body.h"
#include <math.h>
#include <iostream>
#include "Vector2D.h"
#include "Frame Rate Controller.h"
extern FrameRateController * gpFrc;

//Main Shape Class
Shape::Shape(ShapeType Type)
{
	mType = Type;
	mpOwnerBody = nullptr;
}

//Shape Circle Class
ShapeCircle::ShapeCircle() : Shape(CIRCLE) {}
ShapeCircle::~ShapeCircle() {}
bool ShapeCircle::TestPoint(float PointX, float PointY) { return false; }

//Shape AABB Class
ShapeAABB::ShapeAABB() : Shape(RECTANGLE) {}
ShapeAABB::~ShapeAABB() {}
bool ShapeAABB::TestPoint(float PointX, float PointY) {	return false; }

//Shape LineSegment
ShapeLineSegment::ShapeLineSegment() : Shape(LINESEGMENT) {}
ShapeLineSegment::~ShapeLineSegment() {}
bool ShapeLineSegment::TestPoint(float PointX, float PointY) { return false; }

//Contact Object Stuff
Contact::Contact() {
	mpBodies[0] = nullptr;
	mpBodies[1] = nullptr;
}

void Contact::ApplyImpulse() {
	Body * b1 = mpBodies[0];
	Body * b2 = mpBodies[1];

	glm::vec2 relative(b2->mVelX - b1->mVelX, b2->mVelY - b1->mVelY);

	float velAlongNormal = glm::dot(relative, normal);

	float e = 0.0f;
	float j = -(1 + e) * velAlongNormal;
	if (b1->mInvMass != 0 || b2->mInvMass != 0)
		j /= b1->mInvMass + b2->mInvMass;

	glm::vec2 impulse;
	impulse.x = normal.x * j;
	impulse.y = normal.y * j;

	glm::vec2 scaled_impulse;

	scaled_impulse.x = impulse.x * b1->mInvMass;
	scaled_impulse.y = impulse.y * b1->mInvMass;
	b1->mVelX = b1->mVelX - scaled_impulse.x;
	b1->mVelY = b1->mVelY - scaled_impulse.y;

	scaled_impulse.x = impulse.x * b2->mInvMass;
	scaled_impulse.y = impulse.y * b2->mInvMass;
	b2->mVelX = b2->mVelX + scaled_impulse.x;
	b2->mVelY = b2->mVelY + scaled_impulse.y;

	if (b1->mpOwner->objType == BULLET || b2->mpOwner->objType == BULLET) {
		b1->mVelX = b1->mVelX * 0.05f;
		b2->mVelX = b2->mVelX * 0.05f;
		b1->mVelY = b1->mVelY * 0.05f;
		b2->mVelY = b2->mVelY * 0.05f;
	}
}

void Contact::PositionCorrection() {
	Body * b1 = mpBodies[0];
	Body * b2 = mpBodies[1];

	glm::vec2 correction;
	correction.x = normal.x * penetration;
	correction.y = normal.y * penetration;

	glm::vec2 scaled_correction;

	scaled_correction.x = correction.x * b1->mInvMass;
	scaled_correction.y = correction.y * b1->mInvMass;
	b1->mPosX = b1->mPosX - scaled_correction.x;
	b1->mPosY = b1->mPosY - scaled_correction.y;

	scaled_correction.x = correction.x * b2->mInvMass;
	scaled_correction.y = correction.y * b2->mInvMass;
	b2->mPosX = b2->mPosX + scaled_correction.x;
	b2->mPosY = b2->mPosY + scaled_correction.y;
}

//Pre Declaration
bool CheckCollisionLineToCircle(Shape *pShape1, float Pos1X, float Pos1Y,
	Shape *pShape2, float Pos2X, float Pos2Y, std::list<Contact  *> &Contacts);


//Circle Collision with Circle, AABB and Line
bool CheckCollisionCircleToCircle(Shape *pShape1, float Pos1X, float Pos1Y,
	Shape *pShape2, float Pos2X, float Pos2Y, std::list<Contact  *> &Contacts) {

	//Intersection
	ShapeCircle * c1 = static_cast<ShapeCircle *>(pShape1);
	ShapeCircle * c2 = static_cast<ShapeCircle *>(pShape2);

	Vector2D pCenter0,pCenter1;
	Vector2DSet(&pCenter0, Pos1X, Pos1Y);
	Vector2DSet(&pCenter1, Pos2X, Pos2Y);

	if (Vector2DSquareDistance(&pCenter0, &pCenter1) > 
		(c1->mRadius + c2->mRadius)*(c1->mRadius + c2->mRadius)) {
		return false;
	}
	//Create a new contact and add it
	Contact *pNewContact = new Contact();
	pNewContact->mpBodies[0] = pShape1->mpOwnerBody;
	pNewContact->mpBodies[1] = pShape2->mpOwnerBody;
	Contacts.push_back(pNewContact);
	return true;
}

bool CheckCollisionCircleToAABB(Shape *pShape1, float Pos1X, float Pos1Y,
	Shape *pShape2, float Pos2X, float Pos2Y, std::list<Contact  *> &Contacts) {

	ShapeCircle * r1 = static_cast<ShapeCircle *> (pShape1);
	ShapeAABB * r2 = static_cast<ShapeAABB *>(pShape2);

	if ((Pos1X < (Pos2X - r2->mBottom / 2) - r1->mRadius) ||
		(Pos1X > (Pos2X + r2->mBottom / 2) + r1->mRadius) ||
		(Pos1Y < (Pos2Y - r2->mLeft / 2) - r1->mRadius) ||
		(Pos1Y > (Pos2Y + r2->mLeft / 2) + r1->mRadius))
	{
		return false;
	}

	Contact *pNewContact = new Contact();
	pNewContact->mpBodies[0] = pShape1->mpOwnerBody;
	pNewContact->mpBodies[1] = pShape2->mpOwnerBody;

	float pS1width_extent = r1->mRadius * 0.5f;
	float pS1height_extent = r1->mRadius * 0.5f;

	float pS2width_extent = r2->mBottom * 0.5f;
	float pS2height_extent = r2->mLeft * 0.5f;

	float pS1x = Pos1X; //+ pS1width_extent;
	float pS1y = Pos1Y; //+ pS1height_extent;
	float pS2x = Pos2X; //+ pS2width_extent;
	float pS2y = Pos2Y; //+ pS2height_extent;
					   //
	float tempNormalx = pS1x - pS2x;
	float tempNormaly = pS1y - pS2y;

	float x_overlap = pS1width_extent + pS2width_extent - abs(tempNormalx);
	float y_overlap = pS1height_extent + pS2height_extent - abs(tempNormaly);


	if (x_overlap < y_overlap) {
		if (tempNormalx < 0)
			pNewContact->normal = { 1.0f, 0.0f };
		else
			pNewContact->normal = { -1.0f, 0.0f };

		pNewContact->penetration = x_overlap;
	}
	else {
		if (tempNormaly < 0)
			pNewContact->normal = { 0.0f, 1.0f };
		else
			pNewContact->normal = { 0.0f, -1.0f };

		pNewContact->penetration = y_overlap;
	}

	Contacts.push_back(pNewContact);
	return true;
}

bool CheckCollisionCircleToLine(Shape *pShape1, float Pos1X, float Pos1Y,
	Shape *pShape2, float Pos2X, float Pos2Y, std::list<Contact  *> &Contacts) {
	return CheckCollisionLineToCircle(pShape2, Pos2X, Pos2Y,
		pShape1, Pos1X, Pos1Y, Contacts);
}


//AABB Collision with Circle, AABB and Line
bool CheckCollisionAABBtoCircle(Shape *pShape1, float Pos1X, float Pos1Y,
	Shape *pShape2, float Pos2X, float Pos2Y, std::list<Contact  *> &Contacts) {

	return CheckCollisionCircleToAABB(pShape2, Pos2X, Pos2Y,
		pShape1, Pos1X, Pos1Y, Contacts);
}

bool CheckCollisionAABBToAABB(Shape *pShape1, float Pos1X, float Pos1Y,
	Shape *pShape2, float Pos2X, float Pos2Y, std::list<Contact  *> &Contacts) {

	ShapeAABB * r1 = static_cast<ShapeAABB *>(pShape1);
	ShapeAABB * r2 = static_cast<ShapeAABB *>(pShape2);


	if ((Pos1X + r1->mBottom / 2.0f) < (Pos2X - r2->mBottom / 2.0f) ||
		(Pos1X - r1->mBottom / 2.0f) > (Pos2X + r2->mBottom / 2.0f) ||
		(Pos1Y + r1->mLeft / 2.0f) < (Pos2Y - r2->mLeft / 2.0f) ||
		(Pos1Y - r1->mLeft / 2.0f) > (Pos2Y + r2->mLeft / 2.0f)) {
		return false;
	}

	//Create a new contact and add it
	Contact *pNewContact = new Contact();
	pNewContact->mpBodies[0] = pShape1->mpOwnerBody;
	pNewContact->mpBodies[1] = pShape2->mpOwnerBody;

	/*float Pos1X2 = Pos1X + r1->mBottom;
	float Pos1Y2 = Pos1Y + r1->mLeft;

	float Pos2X2 = Pos2X + r2->mBottom;
	float Pos2Y2 = Pos2Y + r2->mLeft;

	float x_overlap = abs(Pos1X - Pos2X) - abs(Pos1X2 - Pos2X2);
	float y_overlap = abs(Pos1Y - Pos2Y) - abs(Pos1Y2 - Pos2Y2);
*/

	float pS1width_extent = r1->mBottom * 0.5f;
	float pS1height_extent = r1->mLeft * 0.5f;

	float pS2width_extent = r2->mBottom * 0.5f;
	float pS2height_extent = r2->mLeft * 0.5f;

	float pS1x = Pos1X; //+ pS1width_extent;
	float pS1y = Pos1Y; //+ pS1height_extent;
	float pS2x = Pos2X; //+ pS2width_extent;
	float pS2y = Pos2Y; //+ pS2height_extent;
					   //
	float tempNormalx = pS1x - pS2x;
	float tempNormaly = pS1y - pS2y;

	float x_overlap = pS1width_extent + pS2width_extent - abs(tempNormalx);
	float y_overlap = pS1height_extent + pS2height_extent - abs(tempNormaly);


	if (x_overlap < y_overlap) {
		if (tempNormalx < 0)
			pNewContact->normal = { 1.0f, 0.0f };
		else
			pNewContact->normal = { -1.0f, 0.0f };

		pNewContact->penetration = x_overlap;
	}
	else {
		if (tempNormaly < 0)
			pNewContact->normal = { 0.0f, 1.0f };
		else
			pNewContact->normal = { 0.0f, -1.0f };

		pNewContact->penetration = y_overlap;
	}


	Contacts.push_back(pNewContact);
	return true;
}

bool CheckCollisionAABBToLine(Shape *pShape1, float Pos1X, float Pos1Y,
	Shape *pShape2, float Pos2X, float Pos2Y, std::list<Contact  *> &Contacts) {
	return false;
}


//Line Collision with Circle, AABB and Line
bool CheckCollisionLineToCircle(Shape *pShape1, float Pos1X, float Pos1Y,
	Shape *pShape2, float Pos2X, float Pos2Y, std::list<Contact  *> &Contacts) {

	ShapeLineSegment * r1 = static_cast<ShapeLineSegment *>(pShape1);
	ShapeCircle * r2 = static_cast<ShapeCircle *>(pShape2);

	Vector2D  Pe;
	Vector2D  Ps;
	Vector2D  P0, P1;


	Vector2DSet(&P0, r1->mPos0x, r1->mPos0y);
	Vector2DSet(&P1, r1->mPos1x, r1->mPos1y);

	Vector2D normal;
	Vector2DSet(&normal, r1->mNx, r1->mNy);

	Vector2DSet(&Ps, Pos2X, Pos2Y);
	Vector2DSet(&Pe,
		Pos2X + (r2->mpOwnerBody->mVelX * (gpFrc->GetFrameTime() / 1000.0f)),
		Pos2Y + (r2->mpOwnerBody->mVelY * (gpFrc->GetFrameTime() / 1000.0f)));

	Vector2D velocity, extendedPoint, L0L1, L1L0, PiL0, PiL1;
	Vector2DSub(&velocity, &Pe, &Ps);
	float start, end, parallel, time, D;

	start = Vector2DDotProduct(&Ps, &normal);
	end = Vector2DDotProduct(&Pe, &normal);
	parallel = Vector2DDotProduct(&normal, &velocity);

	if ((start - r1->mNdot) < -r2->mRadius)
		D = -r2->mRadius;
	else
		D = r2->mRadius;

	float x = (r1->mNdot - start + D);
	time = x / parallel;

	Vector2DScaleAdd(&extendedPoint, &velocity, &Ps, time);
	Vector2DSub(&L1L0, &P1, &P0);
	Vector2DSub(&L0L1, &P0, &P1);
	Vector2DSub(&PiL0, &extendedPoint, &P0);
	Vector2DSub(&PiL1, &extendedPoint, &P1);



	if (((start - r1->mNdot) < -r2->mRadius && (end - r1->mNdot) < -r2->mRadius) ||
		((start - r1->mNdot) > r2->mRadius && (end - r1->mNdot) > r2->mRadius) ||
		(Vector2DDotProduct(&PiL0, &L1L0) < 0.0f) ||
		(Vector2DDotProduct(&PiL1, &L0L1) < 0.0f)) {
		return false;
	}

	Vector2D Pi;
	Vector2D R;
	Pi.x = extendedPoint.x;
	Pi.y = extendedPoint.y;
	Vector2D i, s, m;
	Vector2DSub(&i, &Pe, &Pi);
	Vector2DScale(&s, &normal, Vector2DDotProduct(&i, &normal));
	Vector2DSub(&m, &i, &s);
	Vector2DSub(&R, &m, &s);
	Vector2DNormalize(&R, &R);



	r2->mpOwnerBody->mVelX = R.x * 100.0f;
	r2->mpOwnerBody->mVelY = R.y * 100.0f;

	//Create a new contact and add it
	Contact *pNewContact = new Contact();
	pNewContact->mpBodies[0] = pShape1->mpOwnerBody;
	pNewContact->mpBodies[1] = pShape2->mpOwnerBody;
	Contacts.push_back(pNewContact);
	return true;
}

bool CheckCollisionLineToAABB(Shape *pShape1, float Pos1X, float Pos1Y,
	Shape *pShape2, float Pos2X, float Pos2Y, std::list<Contact  *> &Contacts) {
	return false;
}

bool CheckCollisionLineToLine(Shape *pShape1, float Pos1X, float Pos1Y,
	Shape *pShape2, float Pos2X, float Pos2Y, std::list<Contact  *> &Contacts) {
	return false;
}


//Collision Manager
CollisionManager::CollisionManager() {
	CollisionFunctions[Shape::CIRCLE][Shape::CIRCLE] = CheckCollisionCircleToCircle;
	CollisionFunctions[Shape::CIRCLE][Shape::RECTANGLE] = CheckCollisionCircleToAABB;
	CollisionFunctions[Shape::CIRCLE][Shape::LINESEGMENT] = CheckCollisionCircleToLine;

	CollisionFunctions[Shape::RECTANGLE][Shape::CIRCLE] = CheckCollisionAABBtoCircle;
	CollisionFunctions[Shape::RECTANGLE][Shape::RECTANGLE] = CheckCollisionAABBToAABB;
	CollisionFunctions[Shape::RECTANGLE][Shape::LINESEGMENT] = CheckCollisionAABBToLine;

	CollisionFunctions[Shape::LINESEGMENT][Shape::CIRCLE] = CheckCollisionLineToCircle;
	CollisionFunctions[Shape::LINESEGMENT][Shape::RECTANGLE] = CheckCollisionLineToAABB;
	CollisionFunctions[Shape::LINESEGMENT][Shape::LINESEGMENT] = CheckCollisionLineToLine;
}

CollisionManager::~CollisionManager() {
}

void CollisionManager::Reset() {
	for (auto c : mContacts) {
		delete c;
	}
	mContacts.clear();
}

bool CollisionManager::CheckCollisionAndGenerateContact(Shape *pShape1, float Pos1X, float Pos1Y,
	Shape *pShape2, float Pos2X, float Pos2Y) {
	return CollisionFunctions[pShape1->mType][pShape2->mType](pShape1, Pos1X, Pos1Y,
		pShape2, Pos2X, Pos2Y, mContacts);
}

