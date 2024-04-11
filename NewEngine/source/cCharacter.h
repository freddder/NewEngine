#pragma once
#include "cSpriteModel.h"
#include "cAnimation.h"
#include "cEntity.h"
#include <memory>

enum eEntityMoveResult;

class cCharacter : public cEntity
{
public:
	cCharacter(glm::vec3 pos, std::string textureName);
	~cCharacter();

#pragma region MOVE

protected:
	std::shared_ptr<cSpriteAnimation> spriteAnimation;
	std::shared_ptr<cModelAnimation> modelAnimation;
	eEntityMoveResult ProcessMovement(eDirection dir, bool run);
public:
	std::shared_ptr<cSpriteModel> model;
	virtual void Move(eDirection dir, bool run) = 0;

#pragma endregion

#pragma region FOLLOW

protected:
	cCharacter* follower;
public:
	void MoveFollower(glm::vec3 newPosition, bool run);
	void SetFollower(cCharacter* newFollower);
	void UnsetFollower();

#pragma endregion
};