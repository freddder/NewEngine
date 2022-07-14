#pragma once

#include "cAnimation.h"
#include <vector>

class cSpriteAnimation : public cAnimation
{
public:
	int& spritePosRef;
	glm::vec3& modelScaleRef;
	int initPos;
	glm::vec3 initScale;

	std::vector<sKeyFrameSprite> keyframes;

	cSpriteAnimation(int& _spriteRef, glm::vec3& _modelScale);
	void AddKeyFrame(sKeyFrameSprite newKeyframe);

	virtual void Process(float deltaTime);
};