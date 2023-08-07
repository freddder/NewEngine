#pragma once
#include "cCharacter.h"

static std::vector<sKeyFrameSprite> KEYFRAMES_RUN_UP
{
	sKeyFrameSprite(0.01f, 13), // right leg
	sKeyFrameSprite(0.14f, 12), // middle
	sKeyFrameSprite(0.28f, 14), // left leg
	sKeyFrameSprite(0.42f, 12), // middle
	sKeyFrameSprite(0.56f, 12)  // middle
};

static std::vector<sKeyFrameSprite> KEYFRAMES_RUN_DOWN
{
	sKeyFrameSprite(0.01f, 16), // right leg
	sKeyFrameSprite(0.14f, 15), // middle
	sKeyFrameSprite(0.28f, 17), // left leg
	sKeyFrameSprite(0.42f, 15), // middle
	sKeyFrameSprite(0.56f, 15)  // middle
};

static std::vector<sKeyFrameSprite> KEYFRAMES_RUN_LEFT
{
	sKeyFrameSprite(0.01f, 19), // right leg
	sKeyFrameSprite(0.14f, 18), // middle
	sKeyFrameSprite(0.28f, 20), // left leg
	sKeyFrameSprite(0.42f, 18), // middle
	sKeyFrameSprite(0.56f, 18)  // middle
};

static std::vector<sKeyFrameSprite> KEYFRAMES_RUN_RIGHT
{
	sKeyFrameSprite(0.01f, 22), // right leg
	sKeyFrameSprite(0.14f, 21), // middle
	sKeyFrameSprite(0.28f, 23), // left leg
	sKeyFrameSprite(0.42f, 21), // middle
	sKeyFrameSprite(0.56f, 21)  // middle
};

class cPlayerCharacter : public cCharacter
{
	eDirection lastDesiredDirection;

public:
	cPlayerCharacter(glm::vec3 position);
	~cPlayerCharacter();

	virtual void Walk(eDirection dir);
	void Run(eDirection dir);
	void StopMovement();
};