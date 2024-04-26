#pragma once

#include "cCharacterEntity.h"

class cPlayerEntity : public cCharacterEntity
{
public:
	cPlayerEntity(glm::vec3 pos);
	~cPlayerEntity();

	glm::vec3 GetModelPosition();
	glm::vec3& GetModelPositionRef();

	void StopMovement();
};