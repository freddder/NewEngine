#pragma once
#include "cSpriteModel.h"
#include "cSpriteAnimation.h"
#include "cModelAnimation.h"

class cCharacter
{
protected:
	cSpriteAnimation* spriteAnimation;
	cModelAnimation* modelAnimation;
	bool switchLeg;

public:
	cSpriteModel* model;

	cCharacter(glm::vec3 position, std::string textureName);
	~cCharacter();

	void Walk(eDirection dir);
};