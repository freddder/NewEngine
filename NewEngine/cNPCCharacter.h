#pragma once
#include "cCharacter.h"

class cNPCCharacter : public cCharacter
{
public:
	cNPCCharacter(glm::vec3 position, std::string textureName);
	~cNPCCharacter();

private:
	bool switchLeg;
public:
	virtual void Move(eDirection dir, bool run);
};