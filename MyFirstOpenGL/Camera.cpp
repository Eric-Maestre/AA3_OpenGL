#include "Camera.h"
#include <iostream>

Camera::Camera()
{
	position = glm::vec3(0.f, 0.f, -1.f);
}

void Camera::Update()
{
	if (IM.GetWPressed()) //W
	{
		position.z += 0.1f;
	}
	else if (IM.GetSPressed()) //S
	{
		position.z -= 0.1f;
	}
	else if (IM.GetAPressed()) //A
	{
		position.x += 0.1f;
	}
	else if (IM.GetDPressed()) //D
	{
		position.x -= 0.1f;
	}
}




