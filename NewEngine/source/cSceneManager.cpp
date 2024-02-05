#include "cSceneManager.h"

cSceneManager* cSceneManager::singleton = NULL;

cSceneManager::cSceneManager()
{
	Pokemon::SpawnData meowstic;
	meowstic.nationalDexNumber = 678;
	meowstic.isStatsGenderBased = true;
	meowstic.minLevel = 20;
	meowstic.minLevel = 30;
	meowstic.spawnChance = 1;

	loadedSpawnData.push_back(meowstic);
}

void cSceneManager::SpawnWildPokemon()
{

}
