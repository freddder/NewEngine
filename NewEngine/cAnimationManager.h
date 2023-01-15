#pragma once

#include "cAnimation.h"
#include <set>

class cAnimationManager
{
	//static cAnimationManager* singleton;
	//cAnimationManager();
	//cAnimationManager(const cAnimationManager& obj) = delete;

	std::set<cAnimation*> animations;

public:

	//static cAnimationManager* GetInstance()
	//{
	//	if (singleton == NULL)
	//	{
	//		singleton = new cAnimationManager();
	//	}

	//	return singleton;
	//}

	void Process(float deltaTime);
	void AddAnimation(cAnimation* newAnimation);
	void RemoveAnimation(cAnimation* animationToRemove);
};