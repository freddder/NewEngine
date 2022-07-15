#pragma once

#include "cAnimation.h"
#include <set>

class cAnimationManager
{
	std::set<cAnimation*> animations;

public:

	void Update(float deltaTime);
	void AddAnimation(cAnimation* newAnimation);
	void RemoveAnimation(cAnimation* animationToRemove);
};