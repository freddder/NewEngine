#include "cWildRoamingPokemon.h"

cWildRoamingPokemon::cWildRoamingPokemon(Pokemon::sRoamingPokemonData& pokemonData, glm::vec3 pos, std::string textureName) : cCharacterEntity(pos)
{
	data = pokemonData;
	spriteModel = new cOverworldPokemonSprite(textureName, pos);
}

cWildRoamingPokemon::~cWildRoamingPokemon()
{
	
}
