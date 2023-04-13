#include "cAnimationManager.h"
#include <vector>

cAnimationManager* cAnimationManager::singleton = NULL;

cAnimationManager::cAnimationManager()
{

}

void cAnimationManager::Process(float deltaTime)
{
	//std::vector<cAnimation*> animationsToDelete;
	//for (std::set<cAnimation*>::iterator it = animations.begin(); it != animations.end(); it++)
	//{
	//	(*it)->Process(deltaTime);

	//	if ((*it)->timer >= (*it)->maxDuration)
	//	{
	//		if ((*it)->isRepeat)
	//			(*it)->timer = 0.f;
	//		else
	//			(*it)->isDone = true;
	//	}

	//	if ((*it)->clearAfterComplete && (*it)->isDone)
	//		animationsToDelete.push_back((*it));
	//}

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

	//for (unsigned int i = 0; i < animationsToDelete.size(); i++)
	//{
	//	animations.erase(animationsToDelete[i]);
	//	delete animationsToDelete[i];
	//}
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
