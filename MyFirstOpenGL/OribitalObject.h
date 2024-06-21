#pragma once
#ifndef ORBITALOBJECT_H
#define ORBITALOBJECT_H

#include "GameObject.h"

class OrbitalObject : public GameObject
{
public:
	//movimiento en circulo
	float radius = 0.f;
	float angle = 0.f;

	OrbitalObject() {};

	void Update(float deltaTime);

	void UpdateOpposite(float deltaTime, const OrbitalObject& other);
};

#endif

