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
	int ProcessMovement(eDirection dir, bool run);
public:
	cSpriteModel* model;
	virtual void Move(eDirection dir, bool run) = 0;

	// Follow
protected:
	cCharacter* follower;
public:
	void MoveFollower(glm::vec3 newPosition, bool run);
	void SetFollower(cCharacter* newFollower);
	void UnsetFollower();
};