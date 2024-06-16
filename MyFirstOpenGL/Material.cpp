#include "Material.h"

Material Material::LoadMaterial(const std::string& path)
{
	//cargamos el archivo que contiene el material
	std::string materialInfo = PM.Load_File(path);

	//declaro el material de forma temporal
	Material material;

	std::istringstream stream(materialInfo);
	std::string line;

	//Leo linea a linea
	while (std::getline(stream, line))
	{
		std::istringstream lineStream(line);
		std::string prefix;
		lineStream >> prefix;

		//Asignamos ambient
		if (prefix == "Ka")
			lineStream >> material.ambient.r >> material.ambient.g >> material.ambient.b;
		//Asignamos diffuse
		if (prefix == "Kd")
			lineStream >> material.diffuse.r >> material.diffuse.g >> material.diffuse.b;
		//Asignamos emisivo
		if (prefix == "Ke")
			lineStream >> material.emissive.r >> material.emissive.g >> material.emissive.b;
		//Asignamos opacidad
		if (prefix == "d")
			lineStream >> material.opacity;

	}

	return material;
}
