#include "InputManager.h"

float InputManager::lastX = 400.0f; 
float InputManager::lastY = 300.0f; 
bool InputManager::firstMouse = true;
float InputManager::yaw = -90.0f; 
float InputManager::pitch = 0.0f;

void InputManager::Init(GLFWwindow* window)
{
	this->window = window;

	lastKeyPressed = -1;
	keyPressed = -1;
	key = -1;

	wPressed = false;
	aPressed = false;
	sPressed = false;
	dPressed = false;

	//configurar callback del raton
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void InputManager::Update()
{
	lastKeyPressed = keyPressed;

	keyPressed = 0;

	//casos posibles
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		keyPressed = GLFW_KEY_1;
	else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		keyPressed = GLFW_KEY_2;
	else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		keyPressed = GLFW_KEY_3;

	//comprobar ultima tecla pulsada y actual
	//si la actual es 0 y la ultima es != 0, la tecla ha sido pulsada y soltada

	if (keyPressed == 0 && lastKeyPressed != 0)
	{
		key = lastKeyPressed;
	}

	//a diferencia de otras teclas, WASD se mantienen para mayora comodidad
	wPressed = (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS);
	aPressed = (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS);
	sPressed = (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS);
	dPressed = (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS);

	
}

void InputManager::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // Coordenadas invertidas en Y

	lastX = xpos;
	lastY = ypos;

	ProcessMouseMovement(xoffset, yoffset);
}

void InputManager::ProcessMouseMovement(float xoffset, float yoffset)
{
	const float sensitivity = 0.3f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// Asegurarse de que pitch no exceda los límites
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
}
