#include "Camera.h"
#include <iostream>

Camera::Camera()
{
	position = glm::vec3(0.f, 0.f, -1.f);
	yaw = 0.f;
	pitch = 0.f;
}

void Camera::Update()
{

	directionOfView = glm::normalize(front);

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
	{
		IM.EnableCursor();
		IM.SetKeyNull();
	}
	else if (IM.GetKey() == GLFW_KEY_2)
	{
		IM.DisableCursor();
		IM.SetKeyNull();
	}


	if (IM.IsCursorEnabled())
		return;

	//movimiento del raton
	glm::vec2 distance = IM.MouseMovement() * sensivity;

	//calcular yaw y pitch;
	yaw += distance.x;
	pitch += distance.y;

	if (pitch > maxPitch)
		pitch = maxPitch;
	else if (pitch < -maxPitch)
		pitch = -maxPitch;


	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

}




