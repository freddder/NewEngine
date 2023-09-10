#pragma once
#include "cCharacter.h"

class cOverworldPokemon : public cCharacter
{
	eDirection lastDesiredDirection;

public:
	cOverworldPokemon(glm::vec3 position, std::string textureName);
	~cOverworldPokemon();

	virtual void Move(eDirection dir, bool run);
};