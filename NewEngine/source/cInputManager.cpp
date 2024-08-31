#include "cInputManager.h"

#include <GLFW/glfw3.h>
#include <iostream>

#include "Player.h"
#include "cPlayerEntity.h"

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

	cPlayerEntity* player = Player::playerChar;

	sInputAction& upAction = inputActions[IT_UP];
	upAction.ignoreHoldThreshold = true;
	//upAction.PressedAction = []() { std::cout << "Pressed up" << std::endl; };
	upAction.HeldAction = [this, player]() { player->AttemptMovement(UP, inputActions[IT_CANCEL].isDown); };

	sInputAction& downAction = inputActions[IT_DOWN];
	downAction.ignoreHoldThreshold = true;
	//downAction.PressedAction = []() { std::cout << "Pressed down" << std::endl; };
	downAction.HeldAction = [this, player]() { player->AttemptMovement(DOWN, inputActions[IT_CANCEL].isDown); };

	sInputAction& leftAction = inputActions[IT_LEFT];
	leftAction.ignoreHoldThreshold = true;
	//leftAction.PressedAction = []() { std::cout << "Pressed left" << std::endl; };
	leftAction.HeldAction = [this, player]() { player->AttemptMovement(LEFT, inputActions[IT_CANCEL].isDown); };

	sInputAction& rightAction = inputActions[IT_RIGHT];
	rightAction.ignoreHoldThreshold = true;
	//rightAction.PressedAction = []() { std::cout << "Pressed right" << std::endl; };
	rightAction.HeldAction = [this, player]() { player->AttemptMovement(RIGHT, inputActions[IT_CANCEL].isDown); };
}

void cInputManager::Shutdown()
{
}

void cInputManager::ChangeInputState(eInputState newInputState)
{
	if (newInputState == currInputState) return;

	currInputState = newInputState;
}

bool cInputManager::IsInputDown(eInputType type)
{
	if (type == IT_INVALID) return false;

	return inputActions[type].isDown;
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
		{
			it->second.heldTimer = 0.f;
			it->second.PressedAction();
		}

		if (it->second.isDown && it->second.wasDown)
		{
			it->second.heldTimer += deltaTime;

			if ((it->second.ignoreHoldThreshold || it->second.heldTimer >= KEY_HELD_THRESHOLD) && it->second.HeldAction)
				it->second.HeldAction();
		}

		it->second.wasDown = it->second.isDown;
	}
}