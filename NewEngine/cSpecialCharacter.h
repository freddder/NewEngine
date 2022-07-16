#pragma once
#include "cCharacter.h"

class cSpecialCharacter : public cCharacter
{
public:
	cSpecialCharacter(glm::vec3 position);
	~cSpecialCharacter();

	void Run(eDirection dir);
};