#pragma once

#include "cCharacterEntity.h"
#include "PokemonData.h"

class cTamedRoamingPokemon : public cCharacterEntity
{
	Pokemon::sPokemonData data;
public:
	cTamedRoamingPokemon(Pokemon::sPokemonData& pokemonData, glm::vec3 pos);
	~cTamedRoamingPokemon();
};