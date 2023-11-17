#pragma once
#include "cCharacter.h"

class cPlayerCharacter : public cCharacter
{
	eDirection lastDesiredDirection;
	bool switchLeg;

public:
	cPlayerCharacter(glm::vec3 position);
	~cPlayerCharacter();

	virtual void Move(eDirection dir, bool run);
	void StopMovement();
private:
	void SetupSpriteWalk(eDirection dir);
	void SetupSpriteRun(eDirection dir);
};