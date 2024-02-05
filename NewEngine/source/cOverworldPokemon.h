#pragma once
#include "cCharacter.h"

class cOverworldPokemon : public cCharacter
{
public:
	cOverworldPokemon(glm::vec3 position, std::string textureName);
	~cOverworldPokemon();

private:
	eDirection lastDesiredDirection;
public:
	virtual void Move(eDirection dir, bool run);
};