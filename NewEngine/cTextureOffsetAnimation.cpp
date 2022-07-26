#include "cTextureOffsetAnimation.h"

cTextureOffsetAnimation::cTextureOffsetAnimation(glm::vec2& _modelUVOffset):
	modelUVOffsetRef(_modelUVOffset)
{
	initUVOffset = _modelUVOffset;
}

void cTextureOffsetAnimation::AddKeyFrame(sKeyFrameVec3 newKeyframe)
{
	if (newKeyframe.time > maxDuration)
		maxDuration = newKeyframe.time;

	keyframes.push_back(newKeyframe);
}

void cTextureOffsetAnimation::Reset()
{
	timer = 0.f;
	isDone = false;
	maxDuration = 0.f;
	keyframes.clear();
}

void cTextureOffsetAnimation::Reset(glm::vec2 newInitUVOffset)
{
	this->Reset();
	initUVOffset = newInitUVOffset;
}

void cTextureOffsetAnimation::Process(float deltaTime)
{
	if (isDone)
		return;

	timer += deltaTime * speed;

	sKeyFrameVec3 currKeyframe;
	sKeyFrameVec3 nextKeyframe;
	float posFraction = 1;
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
				currKeyframe.value = glm::vec3(initUVOffset, 0.f);
			}

			posFraction = (timer - currKeyframe.time) / (nextKeyframe.time - currKeyframe.time);

			//switch (nextPosKeyframe.easingType)
			//{
			//case EasingType::EaseIn:
			//	posFraction = glm::sineEaseIn(posFraction);
			//	break;
			//case EasingType::EaseOut:
			//	posFraction = glm::sineEaseOut(posFraction);
			//	break;
			//case EasingType::EaseInOut:
			//	posFraction = glm::sineEaseInOut(posFraction);
			//	break;
			//default:
			//	break;
			//}

			glm::vec3 newOffset = currKeyframe.value + (nextKeyframe.value - currKeyframe.value) * posFraction;
			modelUVOffsetRef.x = glm::sin(glm::radians(newOffset.x)) / 2;
			modelUVOffsetRef.y = glm::sin(glm::radians(newOffset.y)) / 2;

			break;
		}
	}

	if (keyframes.size() != 0)
	{
		if (timer >= maxDuration)
		{
			glm::vec3 finalPosition = keyframes[keyframes.size() - 1].value;
			modelUVOffsetRef = finalPosition;
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
