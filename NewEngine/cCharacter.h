#pragma once
#include "cRenderModel.h"
#include "cSpriteAnimation.h"
#include "cModelAnimation.h"

class cCharacter
{
	cSpriteAnimation* spriteAnimation;
	cModelAnimation* modelAnimation;
	bool switchLeg;

public:
	cRenderModel* model;

	cCharacter(glm::vec3 position, std::string textureName);
	~cCharacter();

	void Walk(eDirection dir);
};