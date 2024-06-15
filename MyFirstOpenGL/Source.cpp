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
#include "Mesh.h"
#include "ProgramManager.h"
#include "Material.h"
#include "Camera.h"
#include "Model.h"

#define WINDOW_WIDTH_DEFAULT 640
#define WINDOW_HEIGHT_DEFAULT 480

int windowWidth = WINDOW_WIDTH_DEFAULT;
int windowHeight = WINDOW_HEIGHT_DEFAULT;

std::vector<Model> models;


void Resize_Window(GLFWwindow* window, int iFrameBufferWidth, int iFrameBufferHeight) {
	//Definir nuevo tamaño del viewport
	glViewport(0, 0, iFrameBufferWidth, iFrameBufferHeight);

	windowWidth = iFrameBufferWidth;
	windowHeight = iFrameBufferHeight;
}


void main() {

	//Definir semillas del rand según el tiempo
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

	//Asignamos función de callback para cuando el frame buffer es modificado
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

	//Configurar la función de mexcla para la opacidad
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Leer textura
	int width, height, nrChannels;
	unsigned char* textureInfo = stbi_load("Assets/Textures/troll.png", &width, &height, &nrChannels, 0);

	//Inicializamos GLEW y controlamos errores
	if (glewInit() == GLEW_OK) {

		//crear camara
		Camera mainCamera;

		//crear modelos
		models.push_back(Model("MyFirstFragmentShader.glsl", "MyFirstGeometryShader.glsl", "MyFirstVertexShader.glsl", "Assets/Models/troll.obj", "Assets/Materials/troll.mtl"));

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

		//Indicar a la tarjeta GPU que programa debe usar
		glUseProgram(models[0].GetProgram());

		models[0].position = glm::vec3(0.f, 0.f, 1.f);
		models[0].rotation = glm::vec3(0.f, 180.f, 0.f);
		models[0].scale = glm::vec3(1.f);

		// Definir la matriz de vista
		glm::mat4 view = glm::lookAt(mainCamera.position, glm::vec3(0.0f, 1.0f, 0.f), mainCamera.localVectorUp);

		// Definir la matriz proyeccion
		glm::mat4 projection = glm::perspective(glm::radians(mainCamera.fFov), (float)windowWidth / (float)windowHeight, mainCamera.fNear, mainCamera.fFar);

		//Asignar valores iniciales al programa
		glUniform2f(glGetUniformLocation(models[0].GetProgram(), "windowSize"), windowWidth, windowHeight);

		//Asignar valor variable de textura a usar.
		glUniform1i(glGetUniformLocation(models[0].GetProgram(), "textureSampler"), 0);

		// Pasar las matrices
		glUniformMatrix4fv(glGetUniformLocation(models[0].GetProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(models[0].GetProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		//Pasar los valores del material



		//Generamos el game loop
		while (!glfwWindowShouldClose(window)) {

			//Pulleamos los eventos (botones, teclas, mouse...)
			glfwPollEvents();
			
			//Limpiamos los buffers
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			//Renderizo objeto 0
			models[0].Update();

			//Cambiamos buffers
			glFlush();
			glfwSwapBuffers(window);
		}

		//Desactivar y eliminar programa
		glUseProgram(0);
		glDeleteProgram(models[0].GetProgram());

	}
	else {
		std::cout << "Ha petao." << std::endl;
		glfwTerminate();
	}

	//Finalizamos GLFW
	glfwTerminate();

}