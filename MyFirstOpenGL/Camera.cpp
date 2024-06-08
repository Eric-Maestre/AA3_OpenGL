#include "Camera.h"
#include <iostream>

Camera::Camera()
{
	position = glm::vec3(0.f, 0.f, -1.f);
}

void Camera::Update()
{
	int stateNumber = IM.GetKey();

	if (stateNumber == 57 || stateNumber == 77) //W
	{
		position.z += 0.1f;
	}
	if (stateNumber == 53 || stateNumber == 73) //S
	{
		position.z -= 0.1f;
	}
	if (stateNumber == 41 || stateNumber == 61) //A
	{
		position.x -= 0.1f;
	}
	if (stateNumber == 44 || stateNumber == 64) //D
	{
		position.x += 0.1f;
	}
}




