#include "cAnimationManager.h"

void cAnimationManager::Update(float deltaTime)
{
	for (unsigned int i = 0; i < animations.size(); i++)
	{
		animations[i]->Process(deltaTime);
	}
}

void cAnimationManager::AddAnimation(cAnimation* newAnimation)
{
	animations.push_back(newAnimation);
}
