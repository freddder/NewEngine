#pragma once
#include "cCharacter.h"

class cPlayerCharacter : public cCharacter
{
	bool putLegForward;

public:
	cPlayerCharacter(glm::vec3 position);
	~cPlayerCharacter();

	void Run(eDirection dir);
};