#pragma once
#include <list>
#include <glm/glm.hpp>

class Body;
class Shape
{
public:
	enum ShapeType {
		CIRCLE,
		RECTANGLE,
		LINESEGMENT,
		NUM
	};
	Shape(ShapeType Type);
	virtual ~Shape() {}
	virtual bool TestPoint(float PointX, float PointY) = 0;
	virtual Shape * ShapeVirtual() = 0;
public:
	Body * mpOwnerBody;
	ShapeType mType;
};

// ---------------------------------

class ShapeCircle : public Shape {
public:
	ShapeCircle();
	~ShapeCircle();
	bool TestPoint(float PointX, float PointY);
	virtual Shape * ShapeVirtual() {
		return new ShapeCircle(*this);
	}
public:
	float mRadius;
};

class ShapeLineSegment :public Shape {
public:
	ShapeLineSegment();
	~ShapeLineSegment();
	bool TestPoint(float PointX, float PointY);
	virtual Shape * ShapeVirtual() {
		return new ShapeLineSegment(*this);
	}
public:
	float mLength;
	float mPos0x, mPos1x;
	float mPos0y, mPos1y;
	float mNx,mNy;
	float mNdot;
};

// --------------------------------

class ShapeAABB : public Shape {
public:
	ShapeAABB();
	~ShapeAABB();
	bool TestPoint(float PointX, float PointY);
	virtual Shape * ShapeVirtual() {
		return new ShapeAABB(*this);
	}
public:
	float mTop, mBottom, mRight, mLeft;
};

// ------------------------------

class Contact {
public:
	Contact();
	~Contact() {}
	void ApplyImpulse();
	void PositionCorrection();
public:
	Body *mpBodies[2];
	glm::vec2 normal;
	float penetration;
};

// ---------------------------------

class CollisionManager {
public:
	CollisionManager();
	~CollisionManager();
	void Reset();
	bool CheckCollisionAndGenerateContact(Shape *pShape1, float Pos1X, float Pos1Y,
		Shape *pShape2, float Pos2X, float Pos2Y);

public:
	std::list<Contact *> mContacts;
private:
	//2D array of function pointers, used to store thecollison function addresses
	bool(*CollisionFunctions[Shape::NUM][Shape::NUM]) (Shape *pShape1, float Pos1X, float Pos1Y, 
														Shape *pShape2, float Pos2X, float Pos2Y,
														std::list<Contact  *> &Contacts);

};