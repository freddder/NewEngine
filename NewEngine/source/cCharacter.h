#pragma once
#include "cSpriteModel.h"
#include "cAnimation.h"
#include <memory>

class cCharacter
{
public:
	cCharacter(glm::vec3 position, std::string textureName);
	~cCharacter();

	// Move
protected:
	std::shared_ptr<cSpriteAnimation> spriteAnimation;
	std::shared_ptr<cModelAnimation> modelAnimation;
	int ProcessMovement(eDirection dir, bool run);
public:
	std::shared_ptr<cSpriteModel> model;
	virtual void Move(eDirection dir, bool run) = 0;

	// Follow
protected:
	cCharacter* follower;
public:
	void MoveFollower(glm::vec3 newPosition, bool run);
	void SetFollower(cCharacter* newFollower);
	void UnsetFollower();
};