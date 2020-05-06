/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Vector2D
Purpose: Vector functions
Language: C++
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: CS529_dhrumilshukla_GameEngine1.0
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Creation date: 09/15/2018
- End Header --------------------------------------------------------*/

#include "Vector2D.h"
#define EPSILON 0.0001
#define PI      3.1415926535897932384626433832795

// ---------------------------------------------------------------------------
double squareRoot(double n) {
	double i, precision = 0.00001;
	for (i = 1; i*i <= n; ++i);           //Integer part
	for (--i; i*i < n; i += precision);  //Fractional part
	return i;
}

void Vector2DZero(Vector2D *pResult)
{
	pResult->x = 0;
	pResult->y = 0;
}

// ---------------------------------------------------------------------------

void Vector2DSet(Vector2D *pResult, float x, float y)
{
	pResult->x = x;
	pResult->y = y;
}

// ---------------------------------------------------------------------------

void Vector2DNeg(Vector2D *pResult, Vector2D *pVec0)
{
	pResult->x = -pVec0->x;
	pResult->y = -pVec0->y;
}

// ---------------------------------------------------------------------------

void Vector2DAdd(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1)
{
	pResult->x = pVec0->x + pVec1-> x;
	pResult->y = pVec0->y + pVec1-> y;
}

// ---------------------------------------------------------------------------

void Vector2DSub(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1)
{
	pResult->x = pVec0->x - pVec1->x;
	pResult->y = pVec0->y - pVec1->y;
}

// ---------------------------------------------------------------------------

void Vector2DNormalize(Vector2D *pResult, Vector2D *pVec0)
{
	float d = Vector2DLength(pVec0);
	pResult->x = pVec0->x / d;
	pResult->y = pVec0->y / d;
}

// ---------------------------------------------------------------------------

void Vector2DScale(Vector2D *pResult, Vector2D *pVec0, float c)
{
	pResult->x = pVec0->x * c;
	pResult->y = pVec0->y * c;
}

// ---------------------------------------------------------------------------

void Vector2DScaleAdd(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1, float c)
{
	pResult->x = ( c * pVec0->x ) + pVec1->x;
	pResult->y = ( c * pVec0->y ) + pVec1->y;
}

// ---------------------------------------------------------------------------

void Vector2DScaleSub(Vector2D *pResult, Vector2D *pVec0, Vector2D *pVec1, float c)
{
	pResult->x = (c * pVec0->x) - pVec1->x;
	pResult->y = (c * pVec0->y) - pVec1->y;
}

// ---------------------------------------------------------------------------

float Vector2DLength(Vector2D *pVec0)
{
	return (float) squareRoot((pVec0->x * pVec0->x) + (pVec0->y * pVec0->y));
}

// ---------------------------------------------------------------------------

float Vector2DSquareLength(Vector2D *pVec0)
{
	return (pVec0->x * pVec0->x) + (pVec0->y * pVec0->y);
}

// ---------------------------------------------------------------------------

float Vector2DDistance(Vector2D *pVec0, Vector2D *pVec1)
{
	return (float) squareRoot( ( (pVec0->x - pVec1->x)*(pVec0->x - pVec1->x) ) + ( (pVec0->y - pVec1->y)*(pVec0->y - pVec1->y) ) );
}

// ---------------------------------------------------------------------------

float Vector2DSquareDistance(Vector2D *pVec0, Vector2D *pVec1)
{
	return ( (pVec0->x - pVec1->x)*(pVec0->x - pVec1->x) ) + ( (pVec0->y - pVec1->y)*(pVec0->y - pVec1->y) );
}

// ---------------------------------------------------------------------------

float Vector2DDotProduct(Vector2D *pVec0, Vector2D *pVec1)
{
	return ( (pVec0->x * pVec1->x) + (pVec0->y * pVec1->y) );
}

// ---------------------------------------------------------------------------

void Vector2DFromAngleDeg(Vector2D *pResult, float angle)
{
	angle = (float) ( angle * PI / 180.0 );
	Vector2D temp;
	temp.x = pResult->x;
	temp.y = pResult->y;
	pResult->x = temp.x*cosf(angle) - temp.y*sinf(angle);
	pResult->y = temp.x*sinf(angle) + temp.y*cosf(angle);
}

// ---------------------------------------------------------------------------

void Vector2DFromAngleRad(Vector2D *pResult, float angle)
{
	Vector2D temp;
	temp.x = pResult->x;
	temp.y = pResult->y;
	pResult->x = temp.x*cosf(angle) - temp.y*sinf(angle);
	pResult->y = temp.x*sinf(angle) + temp.y*cosf(angle);
}

// ---------------------------------------------------------------------------
