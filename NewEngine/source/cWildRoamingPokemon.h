#pragma once

#include "cCharacterEntity.h"
#include "PokemonData.h"

class cWildRoamingPokemon : public cCharacterEntity
{
public:
	cWildRoamingPokemon(glm::vec3 pos, std::string formName);
	~cWildRoamingPokemon();

	int nationalDexNumber = 0;
	std::string formName = "";

	Pokemon::eGender gender = Pokemon::NO_GENDER;
	bool isShiny = false;
};