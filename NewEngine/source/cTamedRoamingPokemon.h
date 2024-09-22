#pragma once

#include "cCharacterEntity.h"
#include "PokemonData.h"

class cTamedRoamingPokemon : public cCharacterEntity
{
	Pokemon::sRoamingPokemonData data;
public:
	cTamedRoamingPokemon(Pokemon::sRoamingPokemonData& pokemonData, glm::vec3 pos);
	~cTamedRoamingPokemon();

	void UpdateRoamingData(Pokemon::sRoamingPokemonData& newData);
};