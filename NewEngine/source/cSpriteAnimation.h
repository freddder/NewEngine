#pragma once

#include "cAnimation.h"
#include <vector>

class cSpriteAnimation : public cAnimation
{
public:
	int& spriteIdRef;
	int initId;
	glm::vec3& modelScaleRef;
	glm::vec3 initScale;

	std::vector<sKeyFrameSprite> keyframes;

	cSpriteAnimation(int& _spriteRef, glm::vec3& _modelScale);
	void AddKeyFrame(sKeyFrameSprite newKeyframe);
	void AddKeyFrames(std::vector<sKeyFrameSprite>& newKeyframes);

	virtual void Reset();
	void Reset(int newInitId, glm::vec3 newInitScale);

	virtual void Process(float deltaTime);
};