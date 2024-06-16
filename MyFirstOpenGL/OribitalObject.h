#pragma once
#ifndef ORBITALOBJECT_H
#define ORBITALOBJECT_H

#include "GameObject.h"

class OribitalObject : public GameObject
{
public:
	//movimiento en circulo
	float radius = 0.f;
	float angle = 0.f;

	//giro en sentido contrario
	bool reverse;


	void Update(float deltaTime);
};

#endif

