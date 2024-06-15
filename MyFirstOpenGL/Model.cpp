#include "Model.h"

Model::Model(std::string fragmentPath, std::string geometryPath, std::string vertexPath, std::string meshPath, std::string materialPath)
{
	compiledProgram = PM.CreationOfProgram(fragmentPath, geometryPath, vertexPath);
	myMesh = Mesh::LoadOBJMesh(meshPath);
	myMaterial = Material::LoadMaterial(materialPath);
}

void Model::GenerateAllMatrixs()
{
	modelPosition = GenerateTranslationMatrix(position);
	modelScale = GenerateScaleMatrix(scale);

	if (rotationAngleX)
		modelRotation = GenerateRotationMatrix(rotation, rotation.x);
	else if (rotationAngleY)
		modelRotation = GenerateRotationMatrix(rotation, rotation.y);
	else if (rotationAngleZ)
		modelRotation = GenerateRotationMatrix(rotation, rotation.z);
	else
		std::cout << "Rotation angle not defined" << std::endl;

}

void Model::Update()
{
	glUseProgram(compiledProgram);
	glUniformMatrix4fv(glGetUniformLocation(compiledProgram, "translationMatrix"), 1, GL_FALSE, glm::value_ptr(modelPosition));
	glUniformMatrix4fv(glGetUniformLocation(compiledProgram, "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(modelRotation));
	glUniformMatrix4fv(glGetUniformLocation(compiledProgram, "scaleMatrix"), 1, GL_FALSE, glm::value_ptr(modelScale));

	glUniform1f(glGetUniformLocation(compiledProgram, "opacity"), myMaterial.opacity);
	glUniform3fv(glGetUniformLocation(compiledProgram, "ambient"), 1, glm::value_ptr(myMaterial.ambient));
	glUniform3fv(glGetUniformLocation(compiledProgram, "diffuse"), 1, glm::value_ptr(myMaterial.diffuse));

	myMesh.Render();
}

glm::mat4 Model::GenerateTranslationMatrix(glm::vec3 translation)
{
	return glm::translate(glm::mat4(1.f), translation);
}

glm::mat4 Model::GenerateRotationMatrix(glm::vec3 axis, float degrees)
{
	return glm::rotate(glm::mat4(1.f), glm::radians(degrees), glm::normalize(axis));
}

glm::mat4 Model::GenerateScaleMatrix(glm::vec3 scale)
{
	return glm::scale(glm::mat4(1.f), scale);
}

