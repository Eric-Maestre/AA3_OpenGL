#include "GameObject.h"

void GameObject::Update(float deltaTime)
{
	angle += velocity * deltaTime;

	angle = fmod(angle, 360.f);

	position.x = radius * cos(glm::radians(angle));
	position.y = radius * sin(glm::radians(angle));
}
