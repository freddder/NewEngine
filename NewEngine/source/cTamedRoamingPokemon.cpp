#include "cTamedRoamingPokemon.h"

cTamedRoamingPokemon::cTamedRoamingPokemon(Pokemon::sPokemonData& pokemonData, glm::vec3 pos) : cCharacterEntity(pos)
{
	data = pokemonData;
	spriteModel = new cOverworldPokemonSprite(pokemonData.MakeRoamingTextureName(), pos);
}

cTamedRoamingPokemon::~cTamedRoamingPokemon()
{

}
