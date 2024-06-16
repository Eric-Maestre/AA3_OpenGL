#pragma once
#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <GLFW/glfw3.h>

#define IM InputManager::Instance()

class InputManager
{
private:
	GLFWwindow* window;

	//ints ultima y actual tecla pulsada
	int key;
	int keyPressed;
	int lastKeyPressed;

	//bools teclas WASD
	bool wPressed, aPressed, sPressed, dPressed;

	InputManager() = default;
	InputManager(InputManager&) = delete;
	InputManager& operator= (const InputManager&) = delete;

	//variables para el rat�n
	static float lastX, lastY;
	static bool firstMouse;
	static float yaw, pitch;

	static void ProcessMouseMovement(float xoffset, float yoffset);

	//variable para raton
	bool cursorEnabled;

public:

	inline static InputManager& Instance()
	{
		static InputManager im;
		return im;
	}

	void Init(GLFWwindow* window);
	void Update();

	//getter de la key para la camara
	int GetKey() { return key; }

	//setter de la key por si acaso
	void SetKey(int a) { key = a; }

	//getters teclas WASD
	bool GetWPressed() { return wPressed; }
	bool GetAPressed() { return aPressed; }
	bool GetSPressed() { return sPressed; }
	bool GetDPressed() { return dPressed; }

	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);

	//getters de yaw y Pitch
	static float GetYaw() { return yaw; }
	static float GetPitch() { return pitch; }

	// Descativar y activar el cursor
	void EnableCursor();
	void DisableCursor();

	//getter estado raton
	bool IsCursorEnabled() { return cursorEnabled; }

	void SetKeyNull() { key = -1; }

};

#endif 