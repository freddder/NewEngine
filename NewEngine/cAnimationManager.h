#pragma once

#include "cAnimation.h"
#include <vector>
#include <map>

struct sEntitySpriteAnimationPreset
{
	std::string animationName;
	std::vector<sKeyFrameSprite> keyframes;
};

class cAnimationManager
{
	static cAnimationManager* singleton;
	cAnimationManager();
	cAnimationManager(const cAnimationManager& obj) = delete;

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

private:
	std::vector<cAnimation*> animations;
	std::map<eSpriteEntityType, std::vector<sEntitySpriteAnimationPreset>> entitySpriteAnimationPresets;

public:
	void Process(float deltaTime);
	void InitializeAnimationsPresets();
	void AddAnimation(cAnimation* newAnimation);
	void RemoveAnimation(cAnimation* animationToRemove);

	void CreateSpriteAnimation(eSpriteEntityType spriteType, std::string animationName, std::vector<sKeyFrameSprite>& keyframes);
	bool GetSpriteAnimationKeyframes(eSpriteEntityType spriteType, std::string animationName, std::vector<sKeyFrameSprite>& keyframes);
};