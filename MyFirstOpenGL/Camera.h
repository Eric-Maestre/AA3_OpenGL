#pragma once
#ifndef CAMERA_H
#define CAMERA_H	

#include "GameObject.h"
#include "InputManager.h"

class Camera : public GameObject
{
public:
	glm::vec3 localVectorUp = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 directionOfView = position + glm::vec3(0.f,0.f, 1.f);

	float fFov = 90.f;
	float fNear = 0.1f;
	float fFar = 10.f;

	//variables raton
	float yaw;
	float pitch;
	float sensivity = 0.3f;
	float maxPitch = 180.f;

	Camera();
	void Update();

	//variable para el movimiento con el raton
	glm::vec3 front = glm::vec3(0.f);
};


#endif


