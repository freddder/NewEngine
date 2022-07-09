#include "cSpriteAnimation.h"

cSpriteAnimation::cSpriteAnimation(int& _spriteRef):
	spritePosRef(_spriteRef)
{
	initPos = _spriteRef;
}

void cSpriteAnimation::AddKeyFrame(sKeyFrameInt newKeyframe)
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
				spritePosRef = keyframes[keyFrameIndex - 1].value;
			else
				spritePosRef = initPos;

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
