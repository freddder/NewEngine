#include "cAnimationManager.h"
#include <vector>

cAnimationManager* cAnimationManager::singleton = NULL;

cAnimationManager::cAnimationManager()
{

}

void cAnimationManager::Process(float deltaTime)
{
	for (int i = animations.size() - 1; i >= 0; i--)
	{
		animations[i]->Process(deltaTime);

		if (animations[i]->timer >= animations[i]->maxDuration)
		{
			if (animations[i]->isRepeat)
				animations[i]->timer = 0.f;
			else
			{
				animations[i]->isDone = true;
				
				if(animations[i]->callback)
					animations[i]->callback();
			}
		}

		if (animations[i]->clearAfterComplete && animations[i]->isDone)
		{
			cAnimation* animationToDelete = animations[i];
			animations.erase(animations.begin() + i);
			delete animationToDelete;
		}
	}
}

void cAnimationManager::AddAnimation(cAnimation* newAnimation)
{
	animations.push_back(newAnimation);
}

void cAnimationManager::RemoveAnimation(cAnimation* animationToRemove)
{
	for (int i = 0; i < animations.size(); i++)
	{
		if (animationToRemove == animations[i])
		{
			animations.erase(animations.begin() + i);
			return;
		}
	}
}
