#pragma once
#include "cSpriteModel.h"
#include "cSpriteAnimation.h"
#include "cModelAnimation.h"

static std::vector<sKeyFrameSprite> KEYFRAMES_WALK_UP_L
{
	sKeyFrameSprite(0.01f, 1), // right leg
	sKeyFrameSprite(0.2f, 0), // middle
	sKeyFrameSprite(0.3f, 0)
};

static std::vector<sKeyFrameSprite> KEYFRAMES_WALK_UP_R
{
	sKeyFrameSprite(0.01f, 2), // left leg
	sKeyFrameSprite(0.2f, 0), // middle
	sKeyFrameSprite(0.3f, 0)
};

static std::vector<sKeyFrameSprite> KEYFRAMES_WALK_DOWN_L
{
	sKeyFrameSprite(0.01f, 4), // right leg
	sKeyFrameSprite(0.2f, 3), // middle
	sKeyFrameSprite(0.3f, 3) // middle
};

static std::vector<sKeyFrameSprite> KEYFRAMES_WALK_DOWN_R
{
	sKeyFrameSprite(0.01f, 5), // left leg
	sKeyFrameSprite(0.2f, 3), // middle
	sKeyFrameSprite(0.3f, 3) // middle
};

static std::vector<sKeyFrameSprite> KEYFRAMES_WALK_LEFT_L
{
	sKeyFrameSprite(0.01f, 7), // right leg
	sKeyFrameSprite(0.2f, 6), // middle
	sKeyFrameSprite(0.3f, 6) // middle
};

static std::vector<sKeyFrameSprite> KEYFRAMES_WALK_LEFT_R
{
	sKeyFrameSprite(0.01f, 8), // left leg
	sKeyFrameSprite(0.2f, 6), // middle
	sKeyFrameSprite(0.3f, 6) // middle
};

static std::vector<sKeyFrameSprite> KEYFRAMES_WALK_RIGHT_L
{
	sKeyFrameSprite(0.01f, 10), // right leg
	sKeyFrameSprite(0.2f, 9), // middle
	sKeyFrameSprite(0.3f, 9)
};

static std::vector<sKeyFrameSprite> KEYFRAMES_WALK_RIGHT_R
{
	sKeyFrameSprite(0.01f, 11), // left leg
	sKeyFrameSprite(0.2f, 9), // middle
	sKeyFrameSprite(0.3f, 9)
};

class cCharacter
{
protected:
	cSpriteAnimation* spriteAnimation;
	cModelAnimation* modelAnimation;
	bool switchLeg;

public:
	cSpriteModel* model;

	cCharacter(glm::vec3 position, std::string textureName);
	~cCharacter();

	virtual void Walk(eDirection dir);
};