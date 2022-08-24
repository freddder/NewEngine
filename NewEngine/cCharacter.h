#pragma once
#include "cRenderModel.h"
#include "cSpriteAnimation.h"
#include "cModelAnimation.h"

class cCharacter
{
	cRenderModel* model;
	cSpriteAnimation* spriteAnimation;
	cModelAnimation* modelAnimation;
	bool switchLeg;

public:
	cCharacter(glm::vec3 position, std::string textureName);
	~cCharacter();

	void Walk(eDirection dir);
};