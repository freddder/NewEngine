#include "cSpriteAnimation.h"

cSpriteAnimation::cSpriteAnimation(int& _spriteRef, glm::vec3& _modelScale) :
	spritePosRef(_spriteRef),
	modelScaleRef(_modelScale)
{
	initPos = _spriteRef;
}

void cSpriteAnimation::AddKeyFrame(sKeyFrameSprite newKeyframe)
{
	if (newKeyframe.time > maxDuration)
		maxDuration = newKeyframe.time;

	keyframes.push_back(newKeyframe);
}

void cSpriteAnimation::Process(float deltaTime)
{
	if (isDone)
		return;

	timer += deltaTime * speed;

	for (unsigned int keyFrameIndex = 0; keyFrameIndex < keyframes.size(); keyFrameIndex++)
	{
		if (keyframes[keyFrameIndex].time >= timer)
		{
			// no interpolation I guess
			if (keyFrameIndex != 0)
			{
				spritePosRef = keyframes[keyFrameIndex - 1].value;

				if ((keyframes[keyFrameIndex - 1].flip && modelScaleRef.x > 0) ||
					!keyframes[keyFrameIndex - 1].flip && modelScaleRef.x < 0)
				{
					modelScaleRef.x *= -1;
				}
			}
			else
			{
				spritePosRef = initPos;
			}

			break;
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
