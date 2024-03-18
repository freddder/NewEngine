#include "cAnimation.h"

cAnimation::cAnimation()
{
	timer = 0.f;
	isDone = false;
	maxDuration = 0.f;
	speed = 1.f;
	isRepeat = false;
	removeAfterComplete = false;
}

cAnimation::~cAnimation()
{
}

void cAnimation::Reset()
{
	timer = 0.f;
	isDone = false;
	maxDuration = 0.f;
	callback = nullptr;
}

cFloatAnimation::cFloatAnimation(float& _valueRef) :
	valueRef(_valueRef)
{
	initValue = _valueRef;
}

void cFloatAnimation::AddKeyFrame(sKeyFrameFloat newKeyframe)
{
	if (newKeyframe.time > maxDuration)
		maxDuration = newKeyframe.time;

	keyframes.push_back(newKeyframe);
}

void cFloatAnimation::Process(float deltaTime)
{
	if (isDone) return;

	timer += deltaTime * speed;

	sKeyFrameFloat currKeyframe;
	sKeyFrameFloat nextKeyframe;
	float fraction = 1;
	for (unsigned int keyFrameIndex = 0; keyFrameIndex < keyframes.size(); keyFrameIndex++)
	{
		if (keyframes[keyFrameIndex].time >= timer)
		{
			nextKeyframe = keyframes[keyFrameIndex];

			if (keyFrameIndex != 0)
			{
				currKeyframe = keyframes[keyFrameIndex - 1];
			}
			else
			{
				currKeyframe.time = 0.f;
				currKeyframe.value = initValue;
			}

			fraction = (timer - currKeyframe.time) / (nextKeyframe.time - currKeyframe.time);

			float newValue = currKeyframe.value + (nextKeyframe.value - currKeyframe.value) * fraction;
			valueRef = newValue;

			break;
		}
	}

	if (keyframes.size() != 0 && timer >= maxDuration)
	{
		float finalDegree = keyframes[keyframes.size() - 1].value;
		valueRef = finalDegree;		
	}
}

cVec3Animation::cVec3Animation(glm::vec3& _valueRef) :
	valueRef(_valueRef)
{
	initValue = _valueRef;
}

void cVec3Animation::AddKeyFrame(sKeyFrameVec3 newKeyframe)
{
	if (newKeyframe.time > maxDuration)
		maxDuration = newKeyframe.time;

	keyframes.push_back(newKeyframe);
}

void cVec3Animation::Process(float deltaTime)
{
	if (isDone) return;

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
			{
				currKeyframe = keyframes[keyFrameIndex - 1];
			}
			else
			{
				currKeyframe.time = 0.f;
				currKeyframe.value = initValue;
			}

			fraction = (timer - currKeyframe.time) / (nextKeyframe.time - currKeyframe.time);

			glm::vec3 newValue = currKeyframe.value + (nextKeyframe.value - currKeyframe.value) * fraction;
			valueRef = newValue;

			break;
		}
	}

	if (keyframes.size() != 0 && timer >= maxDuration)
	{
		glm::vec3 finalValue = keyframes[keyframes.size() - 1].value;
		valueRef = finalValue;
	}
}

cVec4Animation::cVec4Animation(glm::vec4& _valueRef) :
	valueRef(_valueRef)
{
	initValue = _valueRef;
}

void cVec4Animation::AddKeyFrame(sKeyFrameVec4 newKeyframe)
{
	if (newKeyframe.time > maxDuration)
		maxDuration = newKeyframe.time;

	keyframes.push_back(newKeyframe);
}

void cVec4Animation::Process(float deltaTime)
{
	if (isDone) return;

	timer += deltaTime * speed;

	sKeyFrameVec4 currKeyframe;
	sKeyFrameVec4 nextKeyframe;
	float fraction = 1;
	for (unsigned int keyFrameIndex = 0; keyFrameIndex < keyframes.size(); keyFrameIndex++)
	{
		if (keyframes[keyFrameIndex].time >= timer)
		{
			nextKeyframe = keyframes[keyFrameIndex];

			if (keyFrameIndex != 0)
			{
				currKeyframe = keyframes[keyFrameIndex - 1];
			}
			else
			{
				currKeyframe.time = 0.f;
				currKeyframe.value = initValue;
			}

			fraction = (timer - currKeyframe.time) / (nextKeyframe.time - currKeyframe.time);

			glm::vec4 newValue = currKeyframe.value + (nextKeyframe.value - currKeyframe.value) * fraction;
			valueRef = newValue;

			break;
		}
	}

	if (keyframes.size() != 0 && timer >= maxDuration)
	{
		glm::vec4 finalValue = keyframes[keyframes.size() - 1].value;
		valueRef = finalValue;
	}
}

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
	if (isDone)	return;

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
			{
				currScaleKeyframe = scaleKeyframes[keyFrameIndex - 1];
			}
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
}

void cModelAnimation::Reset()
{
	cAnimation::Reset();
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

cSinAnimation::cSinAnimation(glm::vec3& _value, float _valueRange, float _valueOffset) :
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
	cAnimation::Reset();
	keyframes.clear();
}

void cSinAnimation::Reset(glm::vec3 newInitValue)
{
	this->Reset();
	initValue = newInitValue;
}

void cSinAnimation::Process(float deltaTime)
{
	if (isDone)	return;

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
			{
				currKeyframe = keyframes[keyFrameIndex - 1];
			}
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
}

cSpriteAnimation::cSpriteAnimation(int& _spriteRef, glm::vec3& _modelScale) :
	spriteIdRef(_spriteRef),
	modelScaleRef(_modelScale)
{
	initId = _spriteRef;
}

void cSpriteAnimation::AddKeyFrame(sKeyFrameSprite newKeyframe)
{
	if (newKeyframe.time > maxDuration)
		maxDuration = newKeyframe.time;

	keyframes.push_back(newKeyframe);
}

void cSpriteAnimation::AddKeyFrames(std::vector<sKeyFrameSprite>& newKeyframes)
{
	for (int i = 0; i < newKeyframes.size(); i++)
	{
		AddKeyFrame(newKeyframes[i]);
	}
}

void cSpriteAnimation::Reset()
{
	cAnimation::Reset();
	keyframes.clear();
}

void cSpriteAnimation::Reset(int newInitId, glm::vec3 newInitScale)
{
	this->Reset();
	initId = newInitId;
	initScale = newInitScale;
}

void cSpriteAnimation::Process(float deltaTime)
{
	if (isDone)	return;

	timer += deltaTime * speed;

	//if (keyframes.size() != 0 && keyframes[0].time > timer)
	//{
	//	spriteIdRef = initId;
	//	//modelScaleRef = initScale;

	//	return;
	//}

	for (int keyFrameIndex = keyframes.size() - 1; keyFrameIndex >= 0; keyFrameIndex--)
	{
		if (keyframes[keyFrameIndex].time < timer)
		{
			// no interpolation I guess
			spriteIdRef = keyframes[keyFrameIndex].value;

			if ((keyframes[keyFrameIndex].flip && modelScaleRef.z > 0) ||
				!keyframes[keyFrameIndex].flip && modelScaleRef.z < 0)
			{
				modelScaleRef.z *= -1;
			}

			break;
		}
	}
}
