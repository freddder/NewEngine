#include "cTamedRoamingPokemon.h"

cTamedRoamingPokemon::cTamedRoamingPokemon(Pokemon::sPokemonData& pokemonData, glm::vec3 pos, std::string textureName) : cCharacterEntity(pos)
{
	data = pokemonData;
	spriteModel = new cOverworldPokemonSprite(textureName, pos);
}

cTamedRoamingPokemon::~cTamedRoamingPokemon()
{

}
