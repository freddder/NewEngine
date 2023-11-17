#pragma once

#include "cAnimation.h"
#include <vector>

class cSinAnimation : public cAnimation
{
public:
	glm::vec3& valueRef;
	glm::vec3 initValue;

	float valueRange;
	float valueOffset;

	std::vector<sKeyFrameVec3> keyframes;

	cSinAnimation(glm::vec3& _value, float _valueRange, float _valueOffset);
	void AddKeyFrame(sKeyFrameVec3 newKeyframe);

	virtual void Reset();
	void Reset(glm::vec3 newValue);

	virtual void Process(float deltaTime);
};