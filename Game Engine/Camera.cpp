/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Camera.cpp
Purpose: Calculates View Matrix
Language: C++
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: CS529_dhrumilshukla_GameEngine1.0
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Creation date: 11/24/2018
- End Header --------------------------------------------------------*/

#include "Camera.h"
#include "Render Manager.h"
#include "Transform.h"
#include "Frame Rate Controller.h"

extern RenderManager * gpRenderManager;
extern FrameRateController * gpFrc;

Camera::Camera() : Component(CAMERA)
{
}

Camera::~Camera()
{

}

void Camera::Update() {
	Transform * pTr = static_cast<Transform *>(mpOwner->GetComponent(TRANSFORM));
	
	gpRenderManager->mCamX = -pTr->mPosX + 350.0f;

	gpRenderManager->view = glm::translate(glm::mat4(1), glm::vec3(gpRenderManager->mCamX, gpRenderManager->mCamY, 0.0f));
}

Component * Camera::VirtuConst() {
	return new Camera(*this);
}

void Camera::Serialize(GenericObject<false, Value::ValueType> t) {
	gpRenderManager->mCamX = t["posx"].GetFloat();
	gpRenderManager->mCamY = t["posy"].GetFloat();
}