#pragma once

#include "cAnimation.h"
#include <vector>
#include <map>
#include <memory>

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
	std::vector<std::shared_ptr<cAnimation>> animations;
public:
	static void AddAnimation(std::shared_ptr<cAnimation> newAnimation);
	static void Process(float deltaTime);
	static void RemoveAnimation(std::shared_ptr<cAnimation> animationToRemove);

private:
	std::map<eSpriteEntityType, std::vector<sEntitySpriteAnimationPreset>> entitySpriteAnimationPresets;
	void CreateSpritePresetAnimation(eSpriteEntityType spriteType, std::string animationName, std::vector<sKeyFrameSprite>& keyframes);
public:
	void InitializeAnimationsPresets();
	bool GetSpriteAnimationKeyframes(eSpriteEntityType spriteType, std::string animationName, std::vector<sKeyFrameSprite>& keyframes);
};