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

	//configurar callback del raton
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
	else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		keyPressed = GLFW_KEY_F;
	else if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
		keyPressed = GLFW_KEY_0;

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

glm::vec2 InputManager::MouseMovement()
{
	//guardar posicion raton
	glm::vec2 actualMousePosition;
	double x, y;

	//posicion raton
	glfwGetCursorPos(window, &x, &y);

	actualMousePosition = glm::vec2(x, y);

	//distancia posicion actual y anterior
	glm::vec2 distance(actualMousePosition - lastMousePosition);

	//guardar lastMousePosition
	//siguiente iteracion last sera la misma que la actual de esta iteracion
	lastMousePosition = actualMousePosition;

	return distance;
}

void InputManager::EnableCursor()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	cursorEnabled = true;
}

void InputManager::DisableCursor()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	cursorEnabled = false;
}