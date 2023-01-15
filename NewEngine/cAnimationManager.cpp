#include "cAnimationManager.h"
#include <vector>

//cAnimationManager* cAnimationManager::singleton = NULL;

void cAnimationManager::Process(float deltaTime)
{
	std::vector<cAnimation*> animationsToDelete;
	for (std::set<cAnimation*>::iterator it = animations.begin(); it != animations.end(); it++)
	{
		(*it)->Process(deltaTime);

		if ((*it)->clearAfterComplete && (*it)->isDone)
			animationsToDelete.push_back((*it));
	}

	for (unsigned int i = 0; i < animationsToDelete.size(); i++)
	{
		animations.erase(animationsToDelete[i]);
		delete animationsToDelete[i];
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
