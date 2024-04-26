#include "cWildRoamingPokemon.h"

cWildRoamingPokemon::cWildRoamingPokemon(glm::vec3 pos, std::string _formName) : cCharacterEntity(pos)
{
	formName = _formName;
	spriteModel = new cOverworldPokemonSprite(_formName, pos);
}

cWildRoamingPokemon::~cWildRoamingPokemon()
{
	
}
