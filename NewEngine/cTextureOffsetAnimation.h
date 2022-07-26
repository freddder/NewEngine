#pragma once

#include "cAnimation.h"
#include <vector>

class cTextureOffsetAnimation : public cAnimation
{
public:
	glm::vec2& modelUVOffsetRef;
	glm::vec2 initUVOffset;

	std::vector<sKeyFrameVec3> keyframes;

	cTextureOffsetAnimation(glm::vec2& _modelUVOffset);
	void AddKeyFrame(sKeyFrameVec3 newKeyframe);

	virtual void Reset();
	void Reset(glm::vec2 newInitUVOffset);

	virtual void Process(float deltaTime);
};