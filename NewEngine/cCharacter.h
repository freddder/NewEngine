#pragma once
#include "cSpriteModel.h"
#include "cSpriteAnimation.h"
#include "cModelAnimation.h"

class cCharacter
{
public:
	cCharacter(glm::vec3 position, std::string textureName);
	~cCharacter();

	// Move
protected:
	cSpriteAnimation* spriteAnimation;
	cModelAnimation* modelAnimation;
	eSpriteEntityType characterType;
	bool switchLeg;
public:
	cSpriteModel* model;
	virtual void Move(eDirection dir, bool run);

	// Follow
protected:
	cCharacter* follower;
public:
	virtual void Follow(glm::vec3 newPosition, bool run);
	void SetFollower(cCharacter* newFollower);
	void UnsetFollower();
};