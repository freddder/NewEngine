#pragma once

#include "cAnimation.h"
#include <vector>

class cFloatAnimation : public cAnimation
{
public:
	float& valueRef;
	float initValue;

	std::vector<sKeyFrameFloat> keyframes;

	cFloatAnimation(float& _valueRef);
	void AddKeyFrame(sKeyFrameFloat newKeyframe);

	virtual void Process(float deltaTime);
};