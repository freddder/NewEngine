#pragma once
#include <map>
#include <functional>

const float KEY_HELD_THRESHOLD = 0.3f;

enum eInputState
{
	IS_NONE,
	MENU_NAVIGATION,
	OVERWORLD_MOVEMENT,
	// maybe add one for dialog?
};

enum eInputType
{
	IT_INVALID,
	IT_UP,
	IT_DOWN,
	IT_LEFT,
	IT_RIGHT,
	IT_CONFIRM,
	IT_CANCEL,
	IT_MENU
};

struct sInputAction
{
	bool isDown, wasDown; // Per frame basis

	bool ignoreHoldThreshold = false;
	float heldTimer = 0.f;

	std::function<void()> PressedAction; // learning lambdas are fun
	std::function<void()> HeldAction;
};

class cInputManager
{
public:
	cInputManager();
	~cInputManager();

	void Startup();
	void Shutdown();

private:
	eInputState currInputState;
	std::map<eInputState, std::map<eInputType, sInputAction>> inputActions;
public:
	eInputState GetCurrentInputState();
	void ChangeInputState(eInputState newInputState);
	bool IsInputDown(eInputType type);

private:
	std::map<int, eInputType> boundInputs;
public:
	void BindInput(int key, eInputType type);

	void UpdateInput(int key, int action);
	void Process(float deltaTime);
};