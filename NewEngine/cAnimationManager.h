#pragma once

#include "cAnimation.h"
#include <vector>

class cAnimationManager
{
	std::vector<cAnimation*> animations;

public:

	void Update(float deltaTime);
	void AddAnimation(cAnimation* newAnimation);
	// void RemoveAnimation(cAnimation* animationToRemove);
};