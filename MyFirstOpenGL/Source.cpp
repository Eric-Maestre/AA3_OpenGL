#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <stb_image.h>
#include <cstdlib>
#include <ctime>   
#include "Mesh.h"
#include "ProgramManager.h"
#include "Material.h"
#include "Camera.h"
#include "OribitalObject.h"
#include "TimeManager.h"

#define WINDOW_WIDTH_DEFAULT 640
#define WINDOW_HEIGHT_DEFAULT 480

int windowWidth = WINDOW_WIDTH_DEFAULT;
int windowHeight = WINDOW_HEIGHT_DEFAULT;

std::vector<GLuint> compiledPrograms;
std::vector<Mesh> models;
std::vector<Material> materials;

//posiciones posibles
std::vector<glm::vec3> positions;

//funcion que devuelve posicion random
glm::vec3 randomPosition()
{
	if (positions.empty())
		throw std::out_of_range("vector vacio");

	int randomIndex = rand() % positions.size();
	return positions[randomIndex];
}

//minimos y maximos de la escala
//rotacion si o si entre 0 y 360
float minScale = 0.5f;
float maxScale = 2.5f;

//setters max y min
void SetMaxScale(float x) { maxScale = x; }
void SetMinScale(float x) { minScale = x; }

//funciones random rotation y scale
float randomRotation()
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float range = 360.f -1.f;
	return  (random * range);
}

glm::vec3 randomScale()
{
	if(minScale > maxScale)
		throw std::invalid_argument("Min se refiere al mínimo y max al maximo, minimo < maximo, mates de primaria");

	float random = ((float)rand()) / (float)RAND_MAX;
	float range = maxScale - minScale;
	return glm::vec3((random * range) + minScale);
}



//bool para saber si la linterna esta encendida o apagada
bool flashlightOn = false;

void Resize_Window(GLFWwindow* window, int iFrameBufferWidth, int iFrameBufferHeight) {
	//Definir nuevo tama�o del viewport
	glViewport(0, 0, iFrameBufferWidth, iFrameBufferHeight);

	windowWidth = iFrameBufferWidth;
	windowHeight = iFrameBufferHeight;
}

void EnsureMin(float component, float minValue)
{
	if (component < minValue)
		component = minValue;
}

void TurnOnOffFlashlight()
{
	flashlightOn = !flashlightOn;
}


void main() {

	srand(static_cast<unsigned int>(time(nullptr)));

	//Definir semillas del rand seg�n el tiempo
	srand(static_cast<unsigned int>(time(NULL)));

	//Inicializamos GLFW para gestionar ventanas e inputs
	glfwInit();

	//Configuramos la ventana
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//Inicializamos la ventana
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "My Engine", NULL, NULL);

	//Asignamos funci�n de callback para cuando el frame buffer es modificado
	glfwSetFramebufferSizeCallback(window, Resize_Window);

	//Definimos espacio de trabajo
	glfwMakeContextCurrent(window);

	//Permitimos a GLEW usar funcionalidades experimentales
	glewExperimental = GL_TRUE;

	//Activamos cull face
	glEnable(GL_CULL_FACE);

	//Indicamos lado del culling
	glCullFace(GL_BACK);

	//Indicamos lado del culling
	glEnable(GL_DEPTH_TEST);

	//Habilitar blending para activar la opacidad
	glEnable(GL_BLEND);

	//Configurar la funci�n de mexcla para la opacidad
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Leer textura
	int width, height, nrChannels;
	unsigned char* textureInfo = stbi_load("Assets/Textures/troll.png", &width, &height, &nrChannels, 0);

	//Inicializamos GLEW y controlamos errores
	if (glewInit() == GLEW_OK) {

		//crear camara
		Camera mainCamera;

		//crear Time Manager, definir el tiempo del ciclo (20 segundos)
		TimeManager timeManager(20.0f);

		//Iniciar Input Manager
		IM.Init(window);

		//Crear Sol
		OrbitalObject sunPointLight;

		//Crear Luna
		OrbitalObject moonPointLight;

		//bool para saber quien esta arriba
		bool sunActive;

		//valores ambientales globales
		glm::vec3 ambientDay(0.7f, 0.7f, 0.7f); 
		//m�s realismo, iluminacion luna depende SOl
		//en la vida real refleja luz del sol
		float factorReflection = 0.4f;
		glm::vec3 ambientNight = factorReflection * ambientDay;

		//Compilar shaders
		ShaderProgram myFirstProgram;
		myFirstProgram.vertexShader = PM.LoadVertexShader("MyFirstVertexShader.glsl");
		myFirstProgram.geometryShader = PM.LoadGeometryShader("MyFirstGeometryShader.glsl");
		myFirstProgram.fragmentShader = PM.LoadFragmentShader("MyFirstFragmentShader.glsl");

		//Cargo Modelo
		models.push_back(Mesh::LoadOBJMesh("Assets/Models/troll.obj"));

		//Compilar programa
		compiledPrograms.push_back(PM.CreateProgram(myFirstProgram));

		//Compilar materiales
		materials.push_back(Material::LoadMaterial("Assets/Materials/troll.mtl"));

		//Definimos canal de textura activo
		glActiveTexture(GL_TEXTURE0);

		//Generar textura
		GLuint textureID;
		glGenTextures(1, &textureID);

		//Vinculamos texture
		glBindTexture(GL_TEXTURE_2D, textureID);

		//Configurar textura
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//Cargar imagen a la textura
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureInfo);

		//Generar mipmap
		glGenerateMipmap(GL_TEXTURE_2D);

		//Liberar memoria de la imagen cargada
		stbi_image_free(textureInfo);

		//Definimos color para limpiar el buffer de color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Definimos modo de dibujo para cada cara
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//añadir valores para el vector de posiciones
		positions.push_back(glm::vec3(0.f,0.f,1.f));
		positions.push_back(glm::vec3(1.5f, 0.f, 3.f));
		positions.push_back(glm::vec3(-2.6f, 0.f, 6.f));
		positions.push_back(glm::vec3(2.3f, 0.f, 0.5f));
		positions.push_back(glm::vec3(4.0f, 0.f, 1.5f));
		positions.push_back(glm::vec3(0.0f, 0.f, 9.5f));


		//Indicar a la tarjeta GPU que programa debe usar
		glUseProgram(compiledPrograms[0]);

		//Definir la matriz de traslacion, rotacion y escalado
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.f), randomPosition());
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(randomRotation()), glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.f), randomScale());

		//18.f, 18 grados por segundo
		//10 segundos mitad del ciclo y 180 grados
		sunPointLight.position = glm::vec3(-1.f, 0.f, 0.f);
		sunPointLight.radius = 1.f;
		sunPointLight.velocity = 18.f;

		moonPointLight.position = glm::vec3(1.f, 0.f, 0.f);
		moonPointLight.radius = 1.f;
		moonPointLight.velocity = 18.f;
		
		//variable para medir el tiempo 
		double lastTime = glfwGetTime();
		double currentTime;
		float deltaTime;
		

		//Asignar valores iniciales al programa
		glUniform2f(glGetUniformLocation(compiledPrograms[0], "windowSize"), windowWidth, windowHeight);

		//Asignar valor variable de textura a usar.
		glUniform1i(glGetUniformLocation(compiledPrograms[0], "textureSampler"), 0);

		//crear view y projection
		glm::mat4 view;
		glm::mat4 projection;

		//Generamos el game loop
		while (!glfwWindowShouldClose(window)) {

			//Pulleamos los eventos (botones, teclas, mouse...)
			glfwPollEvents();
			
			//Limpiamos los buffers
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			//Update del InputManager
			IM.Update();

			for (int i = 0; i < materials.size(); i++)
			{
				EnsureMin(materials[i].ambient.r, 0.15f);
				EnsureMin(materials[i].ambient.g, 0.15f);
				EnsureMin(materials[i].ambient.b, 0.15f); 
			}

			// Calcular deltaTime
			currentTime = glfwGetTime();
			deltaTime = static_cast<float>(currentTime - lastTime);
			lastTime = currentTime;

			// Actualizar el TimeManager
			timeManager.Update(deltaTime);

			// Actualizar los objetos orbitales
			sunPointLight.Update(deltaTime);
			moonPointLight.UpdateOpposite(deltaTime, sunPointLight);

			glm::vec3 sourceLightPosition;
			bool moonActive;

			if (timeManager.IsDay())
			{
				sourceLightPosition = sunPointLight.position;
				moonActive = false;
			}
			else
			{
				sourceLightPosition = moonPointLight.position;
				moonActive = true;
			}

			if (IM.GetKey() == GLFW_KEY_F)
			{
				TurnOnOffFlashlight();
				IM.SetKeyNull();
			}

			for (int i = 0; i < compiledPrograms.size(); i++)
			{
				glUniform3fv(glGetUniformLocation(compiledPrograms[i], "sourceLight"), 1, glm::value_ptr(sourceLightPosition));
				glUniform1i(glGetUniformLocation(compiledPrograms[i], "moonActive"), moonActive);
				glUniform3fv(glGetUniformLocation(compiledPrograms[i], "ambientDay"), 1, glm::value_ptr(ambientDay));
				glUniform3fv(glGetUniformLocation(compiledPrograms[i], "ambientNight"), 1, glm::value_ptr(ambientNight));

				//variables flashlight
				glUniform3fv(glGetUniformLocation(compiledPrograms[i], "cameraPosition"), 1, glm::value_ptr(mainCamera.position));
				glUniform3fv(glGetUniformLocation(compiledPrograms[i], "cameraDirection"), 1, glm::value_ptr(mainCamera.directionOfView));
				glUniform1f(glGetUniformLocation(compiledPrograms[i], "cameraAngle"), 30.f);
				glUniform1i(glGetUniformLocation(compiledPrograms[i], "flashlightOn"), flashlightOn);

			}

			// Definir la matriz de vista
			view = glm::lookAt(mainCamera.position, mainCamera.directionOfView, mainCamera.localVectorUp);

			// Definir la matriz proyeccion
			projection = glm::perspective(glm::radians(mainCamera.fFov), (float)windowWidth / (float)windowHeight, mainCamera.fNear, mainCamera.fFar);

			// Pasar las matrices
			glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[0], "translationMatrix"), 1, GL_FALSE, glm::value_ptr(translationMatrix));
			glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[0], "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(rotationMatrix));
			glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[0], "scaleMatrix"), 1, GL_FALSE, glm::value_ptr(scaleMatrix));
			glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[0], "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[0], "projection"), 1, GL_FALSE, glm::value_ptr(projection));

			//Pasar los valores del material
			glUniform1f(glGetUniformLocation(compiledPrograms[0], "opacity"), materials[0].opacity);
			glUniform3fv(glGetUniformLocation(compiledPrograms[0], "ambient"), 1, glm::value_ptr(materials[0].ambient));
			glUniform3fv(glGetUniformLocation(compiledPrograms[0], "diffuse"), 1, glm::value_ptr(materials[0].diffuse));

			//Renderizo objeto 0
			models[0].Render();

			//Update Camera
			mainCamera.Update(IM.GetYaw(), IM.GetPitch());

			//Cambiamos buffers
			glFlush();
			glfwSwapBuffers(window);
		}

		//Desactivar y eliminar programa
		glUseProgram(0);
		glDeleteProgram(compiledPrograms[0]);

	}
	else {
		std::cout << "Ha petao." << std::endl;
		glfwTerminate();
	}

	//Finalizamos GLFW
	glfwTerminate();

}