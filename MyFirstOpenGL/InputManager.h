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
	bool wPressed;
	bool aPressed;
	bool sPressed;
	bool dPressed;

	InputManager() = default;
	InputManager(InputManager&) = delete;
	InputManager& operator= (const InputManager&) = delete;


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

};

#endif 