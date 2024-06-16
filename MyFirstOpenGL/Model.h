#pragma once
#ifndef MODEL_H
#define MODEL_H
#include "GameObject.h"
#include "ProgramManager.h"
#include "Mesh.h"
#include "Material.h"

class Model : public GameObject
{
private:
	//programa
	GLuint compiledProgram;
	//Mesh
	Mesh myMesh;
	//Material
	Material myMaterial;

	glm::mat4 modelPosition;
	glm::mat4 modelRotation;
	glm::mat4 modelScale;

	//bool para el eje de rotacion
	bool rotationAngleX = false;
	bool rotationAngleY = true;
	bool rotationAngleZ = false;

	glm::mat4 GenerateTranslationMatrix(glm::vec3 translation);
	glm::mat4 GenerateRotationMatrix(glm::vec3 axis, float degrees);
	glm::mat4 GenerateScaleMatrix(glm::vec3 scale);


public:
	Model(std::string fragmentPath, std::string geometryPath, std::string vertexPath, std::string meshPath, std::string materialPath);

	void GenerateAllMatrixs();

	GLuint GetProgram() { return compiledProgram; }

	void Update();

	void SetRotationAngleX() { rotationAngleX = true; rotationAngleY = false; rotationAngleZ = false; }
	void SetRotationAngleY() { rotationAngleX = false; rotationAngleY = true; rotationAngleZ = false; }
	void SetRotationAngleZ() { rotationAngleX = false; rotationAngleY = false; rotationAngleZ = true; }

};

#endif

