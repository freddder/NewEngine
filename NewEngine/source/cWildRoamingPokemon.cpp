#include "cWildRoamingPokemon.h"

#include "Engine.h"
#include "cSceneManager.h"

cWildRoamingPokemon::cWildRoamingPokemon(Pokemon::sRoamingPokemonData& pokemonData, glm::vec3 pos) : cCharacterEntity(pos)
{
	data = pokemonData;
	spriteModel = new cOverworldPokemonSprite(pokemonData.MakeRoamingTextureName(), pos);
}

cWildRoamingPokemon::~cWildRoamingPokemon()
{
	
}

void cWildRoamingPokemon::WalkInteract()
{
	Manager::scene.EnterWildEncounter(data, this);
}