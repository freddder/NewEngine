#pragma once
#include "cCharacter.h"

class cPlayerCharacter : public cCharacter
{
	eDirection lastDesiredDirection;

public:
	cPlayerCharacter(glm::vec3 position);
	~cPlayerCharacter();

	virtual void Walk(eDirection dir);
	void Run(eDirection dir);
	void StopMovement();
};