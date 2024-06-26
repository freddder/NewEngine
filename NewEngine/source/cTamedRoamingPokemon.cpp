#include "cTamedRoamingPokemon.h"

cTamedRoamingPokemon::cTamedRoamingPokemon(Pokemon::sRoamingPokemonData& pokemonData, glm::vec3 pos) : cCharacterEntity(pos)
{
	data = pokemonData;
	spriteModel = new cOverworldPokemonSprite(pokemonData.MakeRoamingTextureName(), pos);
}

cTamedRoamingPokemon::~cTamedRoamingPokemon()
{

}
