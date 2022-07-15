#pragma once
#include "cModel.h"
#include "cSpriteAnimation.h"
#include "cModelAnimation.h"

class cCharacter
{
	cModel* model;
	cSpriteAnimation* spriteAnimation;
	cModelAnimation* modelAnimation;

public:
	cCharacter();
	~cCharacter();

	void Walk(eDirection dir);
};