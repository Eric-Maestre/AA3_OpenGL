#include "InputManager.h"


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

	wPressed = (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS);
	aPressed = (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS);
	sPressed = (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS);
	dPressed = (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS);

}



