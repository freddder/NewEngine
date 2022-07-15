#pragma once
#include "cModel.h"
#include "cSpriteAnimation.h"
#include "cModelAnimation.h"

class cPlayerCharacter
{
	cModel* model;
	cSpriteAnimation* spriteAnimation;
	cModelAnimation* modelAnimation;

public:
	cPlayerCharacter();
	~cPlayerCharacter();


};