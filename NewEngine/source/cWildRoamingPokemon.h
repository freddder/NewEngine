#pragma once

#include "cCharacterEntity.h"
#include "PokemonData.h"

class cWildRoamingPokemon : public cCharacterEntity
{
	Pokemon::sRoamingPokemonData data;
public:
	cWildRoamingPokemon(Pokemon::sRoamingPokemonData& pokemonData, glm::vec3 pos);
	~cWildRoamingPokemon();

	virtual void WalkInteract();
};