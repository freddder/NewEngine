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