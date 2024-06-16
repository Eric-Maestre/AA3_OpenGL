#include "OribitalObject.h"

void OrbitalObject::Update(float deltaTime)
{
	angle += velocity * deltaTime;

	angle = fmod(angle, 360.f);

	position.x = radius * cos(glm::radians(angle));
	position.y = radius * sin(glm::radians(angle));
}

void OrbitalObject::UpdateOpposite(float deltaTime, const OrbitalObject& other)
{
    //asegurarse que esta en el lado opuesto
    angle = other.angle + 180.0f;

    angle = fmod(angle, 360.f);
    if (angle < 0) {
        angle += 360.f;
    }

    position.x = radius * cos(glm::radians(angle));
    position.y = radius * sin(glm::radians(angle));
}
