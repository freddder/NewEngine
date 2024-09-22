#include "cTamedRoamingPokemon.h"

cTamedRoamingPokemon::cTamedRoamingPokemon(Pokemon::sRoamingPokemonData& pokemonData, glm::vec3 pos) : cCharacterEntity(pos)
{
	data = pokemonData;
	spriteModel = new cOverworldPokemonSprite(pokemonData.MakeRoamingTextureName(), pos);
}

cTamedRoamingPokemon::~cTamedRoamingPokemon()
{

}

void cTamedRoamingPokemon::UpdateRoamingData(Pokemon::sRoamingPokemonData& newData)
{
	data = newData;
	spriteModel->model.get()->textureName = newData.MakeRoamingTextureName();
}
