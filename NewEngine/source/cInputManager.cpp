#include "cInputManager.h"

#include <GLFW/glfw3.h>
#include <iostream>

#include "Player.h"
#include "cPlayerEntity.h"

#include "Engine.h"
#include "cSceneManager.h"

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

	// Setup input actions
	sInputAction& moveUpAction = inputActions[OVERWORLD_MOVEMENT][IT_UP];
	moveUpAction.ignoreHoldThreshold = true;
	moveUpAction.HeldAction = [this]() { 
		Player::playerChar->AttemptMovement(UP, inputActions[currInputState][IT_CANCEL].isDown); 
	};

	sInputAction& moveDownAction = inputActions[OVERWORLD_MOVEMENT][IT_DOWN];
	moveDownAction.ignoreHoldThreshold = true;
	moveDownAction.HeldAction = [this]() { 
		Player::playerChar->AttemptMovement(DOWN, inputActions[currInputState][IT_CANCEL].isDown); 
	};

	sInputAction& moveLeftAction = inputActions[OVERWORLD_MOVEMENT][IT_LEFT];
	moveLeftAction.ignoreHoldThreshold = true;
	moveLeftAction.HeldAction = [this]() { 
		Player::playerChar->AttemptMovement(LEFT, inputActions[currInputState][IT_CANCEL].isDown); 
	};

	sInputAction& moveRightAction = inputActions[OVERWORLD_MOVEMENT][IT_RIGHT];
	moveRightAction.ignoreHoldThreshold = true;
	moveRightAction.HeldAction = [this]() { 
		Player::playerChar->AttemptMovement(RIGHT, inputActions[currInputState][IT_CANCEL].isDown); 
	};

	sInputAction& openMenu = inputActions[OVERWORLD_MOVEMENT][IT_MENU];
	openMenu.PressedAction = [this]() {
		ChangeInputState(MENU_NAVIGATION);
	};

	sInputAction& menuConfirm = inputActions[MENU_NAVIGATION][IT_CONFIRM];
	menuConfirm.PressedAction = [this]() {
		//Manager::scene.RunEncounter();
	};

	sInputAction& menuCancel = inputActions[MENU_NAVIGATION][IT_CANCEL];
	menuCancel.PressedAction = [this]() {
		ChangeInputState(OVERWORLD_MOVEMENT);
	};

	sInputAction& menuUp = inputActions[MENU_NAVIGATION][IT_UP];
	sInputAction& menuDown = inputActions[MENU_NAVIGATION][IT_DOWN];
	sInputAction& menuLeft = inputActions[MENU_NAVIGATION][IT_LEFT];
	sInputAction& menuRight = inputActions[MENU_NAVIGATION][IT_RIGHT];
}

void cInputManager::Shutdown()
{
}

eInputState cInputManager::GetCurrentInputState()
{
	return currInputState;
}

void cInputManager::ChangeInputState(eInputState newInputState)
{
	if (newInputState == currInputState) return;

	// Reset variables
	for (std::map<eInputType, sInputAction>::iterator it = inputActions[currInputState].begin(); it != inputActions[currInputState].end(); it++)
	{
		it->second.isDown = false;
		it->second.wasDown = false;
		it->second.heldTimer = 0.f;
	}

	currInputState = newInputState;
}

bool cInputManager::IsInputDown(eInputType type)
{
	if (type == IT_INVALID) return false;

	return inputActions[currInputState][type].isDown;
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
		inputActions[currInputState][type].isDown = true;
	else if (action == GLFW_RELEASE)
		inputActions[currInputState][type].isDown = false;
}

void cInputManager::Process(float deltaTime)
{
	eInputState state = currInputState;
	for (std::map<eInputType, sInputAction>::iterator it = inputActions[state].begin(); it != inputActions[state].end(); it++)
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