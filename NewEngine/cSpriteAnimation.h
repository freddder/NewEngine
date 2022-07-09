#pragma once

#include "cAnimation.h"
#include <vector>

class cSpriteAnimation : public cAnimation
{
public:
	int& spritePosRef;

	int initPos;

	std::vector<sKeyFrameInt> keyframes;

	cSpriteAnimation(int& _spriteRef);
	void AddKeyFrame(sKeyFrameInt newKeyframe);

	virtual void Process(float deltaTime);
};