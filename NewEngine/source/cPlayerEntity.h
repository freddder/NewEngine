#pragma once

#include "cCharacterEntity.h"

class cPlayerEntity : public cCharacterEntity
{
public:
	cPlayerEntity();
	~cPlayerEntity();

	glm::vec3 GetModelPosition();
	glm::vec3& GetModelPositionRef();
};