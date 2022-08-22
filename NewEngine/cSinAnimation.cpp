#include "cSinAnimation.h"

cSinAnimation::cSinAnimation(glm::vec3& _value, float _valueRange, float _valueOffset):
	valueRef(_value)
{
	initValue = _value;

	valueRange = _valueRange;
	valueOffset = _valueOffset;
}

void cSinAnimation::AddKeyFrame(sKeyFrameVec3 newKeyframe)
{
	if (newKeyframe.time > maxDuration)
		maxDuration = newKeyframe.time;

	keyframes.push_back(newKeyframe);
}

void cSinAnimation::Reset()
{
	timer = 0.f;
	isDone = false;
	maxDuration = 0.f;
	keyframes.clear();
}

void cSinAnimation::Reset(glm::vec3 newInitValue)
{
	this->Reset();
	initValue = newInitValue;
}

void cSinAnimation::Process(float deltaTime)
{
	if (isDone)
		return;

	timer += deltaTime * speed;

	sKeyFrameVec3 currKeyframe;
	sKeyFrameVec3 nextKeyframe;
	float fraction = 1;
	for (unsigned int keyFrameIndex = 0; keyFrameIndex < keyframes.size(); keyFrameIndex++)
	{
		if (keyframes[keyFrameIndex].time >= timer)
		{
			nextKeyframe = keyframes[keyFrameIndex];

			if (keyFrameIndex != 0)
				currKeyframe = keyframes[keyFrameIndex - 1];
			else
			{
				currKeyframe.time = 0.f;
				currKeyframe.value = initValue;
			}

			fraction = (timer - currKeyframe.time) / (nextKeyframe.time - currKeyframe.time);

			glm::vec3 newOffset = currKeyframe.value + (nextKeyframe.value - currKeyframe.value) * fraction;
			valueRef.x = glm::sin(glm::radians(newOffset.x)) * (valueRange / (float)2) + valueOffset;
			valueRef.y = glm::sin(glm::radians(newOffset.y)) * (valueRange / (float)2) + valueOffset;
			valueRef.z = glm::sin(glm::radians(newOffset.z)) * (valueRange / (float)2) + valueOffset;

			break;
		}
	}

	if (keyframes.size() != 0)
	{
		if (timer >= maxDuration)
		{
			glm::vec3 finalPosition = keyframes[keyframes.size() - 1].value;
			valueRef = finalPosition;
		}
	}

	if (timer >= maxDuration)
	{
		if (isRepeat)
			timer = 0.f;
		else
			isDone = true;
	}
}
