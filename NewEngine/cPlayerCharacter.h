#pragma once
#include "cCharacter.h"

class cPlayerCharacter : public cCharacter
{
	bool putLegForwrard;

public:
	cPlayerCharacter(glm::vec3 position);
	~cPlayerCharacter();

	void Run(eDirection dir);
};