#include "cSpriteAnimation.h"

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

void cSpriteAnimation::Reset()
{
	//timer = 0.f;
	//maxDuration = 0.f;
	//isDone = false;
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
	if (isDone)
		return;

	timer += deltaTime * speed;

	//for (unsigned int keyFrameIndex = 0; keyFrameIndex < keyframes.size(); keyFrameIndex++)
	//{
	//	if (keyframes[keyFrameIndex].time >= timer)
	//	{
	//		// no interpolation I guess
	//		if (keyFrameIndex != 0)
	//		{
	//			spriteIdRef = keyframes[keyFrameIndex - 1].value;

	//			if ((keyframes[keyFrameIndex - 1].flip && modelScaleRef.z > 0) ||
	//				!keyframes[keyFrameIndex - 1].flip && modelScaleRef.z < 0)
	//			{
	//				modelScaleRef.z *= -1;
	//			}
	//		}
	//		else
	//		{
	//			spriteIdRef = initId;
	//		}

	//		break;
	//	}
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

	//if (timer >= maxDuration)
	//{
	//	if (isRepeat)
	//		timer = 0.f;
	//	else
	//		isDone = true;
	//}
}
