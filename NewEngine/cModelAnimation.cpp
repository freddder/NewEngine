#include "cModelAnimation.h"

cModelAnimation::cModelAnimation(glm::vec3& _posRef, glm::vec3& _rotRef, glm::vec3& _sclRef) :
	positionRef(_posRef),
	orintationRef(_rotRef),
	scaleRef(_sclRef)
{
	initPosition = _posRef;
	initOrientation = _rotRef;
	initScale = _sclRef;
}

void cModelAnimation::AddPositionKeyFrame(sKeyFrameVec3 newKeyframe)
{
	if (newKeyframe.time > maxDuration)
		maxDuration = newKeyframe.time;

	positionKeyframes.push_back(newKeyframe);
}

void cModelAnimation::AddOrientationKeyFrame(sKeyFrameVec3 newKeyframe)
{
	if (newKeyframe.time > maxDuration)
		maxDuration = newKeyframe.time;

	orientationKeyframes.push_back(newKeyframe);
}

void cModelAnimation::AddScaleKeyFrame(sKeyFrameVec3 newKeyframe)
{
	if (newKeyframe.time > maxDuration)
		maxDuration = newKeyframe.time;

	scaleKeyframes.push_back(newKeyframe);
}

void cModelAnimation::Process(float deltaTime)
{
	if (isDone)
		return;

	timer += deltaTime * speed;

	// Position
	sKeyFrameVec3 currPosKeyframe;
	sKeyFrameVec3 nextPosKeyframe;
	float posFraction = 1;
	for (unsigned int keyFrameIndex = 0; keyFrameIndex < positionKeyframes.size(); keyFrameIndex++)
	{
		if (positionKeyframes[keyFrameIndex].time >= timer)
		{
			nextPosKeyframe = positionKeyframes[keyFrameIndex];

			if (keyFrameIndex != 0)
				currPosKeyframe = positionKeyframes[keyFrameIndex - 1];
			else
			{
				currPosKeyframe.time = 0.f;
				currPosKeyframe.value = initPosition;
			}

			posFraction = (timer - currPosKeyframe.time) / (nextPosKeyframe.time - currPosKeyframe.time);

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

			glm::vec3 newPosition = currPosKeyframe.value + (nextPosKeyframe.value - currPosKeyframe.value) * posFraction;
			positionRef = newPosition;

			break;
		}
	}

	if (positionKeyframes.size() != 0)
	{
		if (timer >= maxDuration)
		{
			glm::vec3 finalPosition = positionKeyframes[positionKeyframes.size() - 1].value;
			positionRef = finalPosition;
		}
	}

	// Orientation


	// Scale
	sKeyFrameVec3 currScaleKeyframe;
	sKeyFrameVec3 nextScaleKeyframe;
	float scaleFraction = 1;
	for (unsigned int keyFrameIndex = 0; keyFrameIndex < scaleKeyframes.size(); keyFrameIndex++)
	{
		if (scaleKeyframes[keyFrameIndex].time >= timer)
		{
			nextScaleKeyframe = scaleKeyframes[keyFrameIndex];

			if (keyFrameIndex != 0)
				currScaleKeyframe = scaleKeyframes[keyFrameIndex - 1];
			else
			{
				currScaleKeyframe.time = 0.f;
				currScaleKeyframe.value = initScale;
			}

			scaleFraction = (timer - currScaleKeyframe.time) / (nextScaleKeyframe.time - currScaleKeyframe.time);

			//switch (nextScaleKeyframe.easingType)
			//{
			//case EasingType::EaseIn:
			//	scaleFraction = glm::sineEaseIn(scaleFraction);
			//	break;
			//case EasingType::EaseOut:
			//	scaleFraction = glm::sineEaseOut(scaleFraction);
			//	break;
			//case EasingType::EaseInOut:
			//	scaleFraction = glm::sineEaseInOut(scaleFraction);
			//	break;
			//default:
			//	break;
			//}

			glm::vec3 newScale = currScaleKeyframe.value + (nextScaleKeyframe.value - currScaleKeyframe.value) * scaleFraction;
			scaleRef = newScale;

			break;
		}
	}

	if (scaleKeyframes.size() != 0)
	{
		if (timer >= maxDuration)
		{
			glm::vec3 finalScale = scaleKeyframes[scaleKeyframes.size() - 1].value;
			scaleRef = finalScale;
		}
	}

	//if (timer >= maxDuration)
	//{
	//	if (isRepeat)
	//		timer = 0.f;
	//	else
	//		isDone = true;
	//}
}

void cModelAnimation::Reset()
{
	timer = 0.f;
	isDone = false;
	maxDuration = 0.f;
	positionKeyframes.clear();
	orientationKeyframes.clear();
	scaleKeyframes.clear();
}

void cModelAnimation::Reset(glm::vec3 newInitPos, glm::vec3 newInitOri, glm::vec3 newInitScale)
{
	this->Reset();
	initPosition = newInitPos;
	initOrientation = newInitOri;
	initScale = newInitScale;
}
