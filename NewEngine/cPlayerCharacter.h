#pragma once
#include "cCharacter.h"

class cPlayerCharacter : public cCharacter
{
public:
	cPlayerCharacter();
	~cPlayerCharacter();

	void Run(eDirection dir);
};