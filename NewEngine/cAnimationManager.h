#pragma once

#include "cAnimation.h"
#include <vector>

class cAnimationManager
{
	static cAnimationManager* singleton;
	cAnimationManager();
	cAnimationManager(const cAnimationManager& obj) = delete;

	std::vector<cAnimation*> animations;

public:

	static cAnimationManager* GetInstance()
	{
		if (singleton == NULL)
		{
			singleton = new cAnimationManager();
		}

		return singleton;
	}

	static void DestroyInstance()
	{
		if (singleton != NULL)
		{
			delete singleton;
			singleton = NULL;
		}
	}

	void Process(float deltaTime);
	void AddAnimation(cAnimation* newAnimation);
	void RemoveAnimation(cAnimation* animationToRemove);
};