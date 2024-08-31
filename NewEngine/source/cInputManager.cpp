#include "cInputManager.h"

#include <GLFW/glfw3.h>
#include <iostream>

cInputManager::cInputManager()
{
	currInputState = OVERWORLD_MOVEMENT;
}

cInputManager::~cInputManager()
{
}

void cInputManager::Startup()
{
	BindInput(GLFW_KEY_UP, IT_UP);
	BindInput(GLFW_KEY_DOWN, IT_DOWN);
	BindInput(GLFW_KEY_LEFT, IT_LEFT);
	BindInput(GLFW_KEY_RIGHT, IT_RIGHT);

	BindInput(GLFW_KEY_Z, IT_CONFIRM);
	BindInput(GLFW_KEY_X, IT_CANCEL);
	BindInput(GLFW_KEY_C, IT_MENU);

	sInputAction& upAction = inputActions[IT_UP];
	upAction.PressedAction = []() { std::cout << "Pressed up" << std::endl; };
	upAction.HeldAction = []() { std::cout << "Holding up" << std::endl; };

	sInputAction& downAction = inputActions[IT_DOWN];
	downAction.PressedAction = []() { std::cout << "Pressed down" << std::endl; };
	downAction.HeldAction = []() { std::cout << "Holding down" << std::endl; };
}

void cInputManager::Shutdown()
{
}

void cInputManager::ChangeInputState(eInputState newInputState)
{
	if (newInputState == currInputState) return;

	currInputState = newInputState;
}

void cInputManager::BindInput(int key, eInputType type)
{
	if (type == IT_INVALID) return;

	boundInputs[key] = type;
}

void cInputManager::UpdateInput(int key, int action)
{
	eInputType type = boundInputs[key];
	if (type == IT_INVALID) return;

	if (action == GLFW_PRESS)
		inputActions[type].isDown = true;
	else if (action == GLFW_RELEASE)
		inputActions[type].isDown = false;
}

void cInputManager::ProcessInputs(float deltaTime)
{
	for (std::map<eInputType, sInputAction>::iterator it = inputActions.begin(); it != inputActions.end(); it++)
	{
		if (it->second.isDown && !it->second.wasDown && it->second.PressedAction)
			it->second.PressedAction();

		if (it->second.isDown && it->second.wasDown && it->second.HeldAction)
			it->second.HeldAction();

		it->second.wasDown = it->second.isDown;
	}
}