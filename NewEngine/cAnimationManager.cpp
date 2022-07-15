#include "cAnimationManager.h"

void cAnimationManager::Update(float deltaTime)
{
	for (std::set<cAnimation*>::iterator it = animations.begin(); it != animations.end(); it++)
	{
		(*it)->Process(deltaTime);
	}
}

void cAnimationManager::AddAnimation(cAnimation* newAnimation)
{
	animations.insert(newAnimation);
}

void cAnimationManager::RemoveAnimation(cAnimation* animationToRemove)
{
	animations.erase(animationToRemove);
}
