#pragma once
#include <string>
#include <glm.hpp>
#include "ProgramManager.h"

#ifndef MATERIAL_H
#define MATERIAL_H

class Material
{
public:
	glm::vec3 ambient = glm::vec3(0.f);
	glm::vec3 diffuse = glm::vec3(0.f);
	glm::vec3 emissive = glm::vec3(0.f);
	float opacity = 1.f;

	Material() = default;

	Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& emissive, float opacity)
		: ambient(ambient), diffuse(diffuse), emissive(emissive), opacity(opacity) {}

	static Material LoadMaterial(const std::string& path);
};

#endif


