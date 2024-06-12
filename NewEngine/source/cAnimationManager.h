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
public:
	cAnimationManager();
	~cAnimationManager();

private:
	std::vector<std::shared_ptr<cAnimation>> animations;
public:
	void AddAnimation(std::shared_ptr<cAnimation> newAnimation);
	void Process(float deltaTime);
	void RemoveAnimation(std::shared_ptr<cAnimation> animationToRemove);

private:
	std::map<eSpriteEntityType, std::vector<sEntitySpriteAnimationPreset>> entitySpriteAnimationPresets;
	void CreateSpritePresetAnimation(eSpriteEntityType spriteType, std::string animationName, std::vector<sKeyFrameSprite>& keyframes);
public:
	void InitializeAnimationsPresets();
	bool GetSpriteAnimationKeyframes(eSpriteEntityType spriteType, std::string animationName, std::vector<sKeyFrameSprite>& keyframes);
};