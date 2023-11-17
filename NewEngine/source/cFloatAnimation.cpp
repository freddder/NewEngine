#include "cFloatAnimation.h"

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
	if (isDone)
		return;

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
				currKeyframe = keyframes[keyFrameIndex - 1];
			else
			{
				currKeyframe.time = 0.f;
				currKeyframe.value = initValue;
			}

			fraction = (timer - currKeyframe.time) / (nextKeyframe.time - currKeyframe.time);

			float degree = currKeyframe.value + (nextKeyframe.value - currKeyframe.value) * fraction;
			valueRef = degree;

			break;
		}
	}

	if (keyframes.size() != 0)
	{
		if (timer >= maxDuration)
		{
			float finalDegree = keyframes[keyframes.size() - 1].value;
			valueRef = finalDegree;
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
