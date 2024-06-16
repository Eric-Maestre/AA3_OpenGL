#include "Camera.h"
#include <iostream>

Camera::Camera()
{
	position = glm::vec3(0.f, 0.f, -1.f);
}

void Camera::Update(float yaw, float pitch)
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

	if (IM.GetKey() == GLFW_KEY_1)
		IM.EnableCursor();
	else if (IM.GetKey() == GLFW_KEY_2)
		IM.DisableCursor();

	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	directionOfView = glm::normalize(front);
}




