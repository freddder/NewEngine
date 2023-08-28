#include "cAnimation.h"

cAnimation::cAnimation()
{
	timer = 0.f;
	isDone = false;
	maxDuration = 0.f;
	speed = 1.f;
	isRepeat = false;
	clearAfterComplete = false;
}

void cAnimation::Reset()
{
	timer = 0.f;
	isDone = false;
	maxDuration = 0.f;
	callback = nullptr;
}