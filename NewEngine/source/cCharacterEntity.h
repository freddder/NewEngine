#pragma once

#include "cEntity.h"
#include "CharacterSprite.h"

class cCharacterEntity : public cEntity
{
public:
	cCharacterEntity(glm::vec3 pos);
	~cCharacterEntity();

	// Move
	cCharacterSprite* spriteModel;
	void AttemptMovement(eDirection dir, bool run);

	// Follow
private:
	cCharacterEntity* follower;
public:
	void SetFollower(cCharacterEntity* newFollower);
	void UnsetFollower();
	void MoveFollower(glm::vec3 newPos, bool run);
};