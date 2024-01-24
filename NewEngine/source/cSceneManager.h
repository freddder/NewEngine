#pragma once

// purpose: load necessary textures and 3d models and deal with rendering
#include <string>
#include "PokemonData.h"

class cSceneManager
{
	// Singleton
	static cSceneManager* singleton;
	cSceneManager();
	cSceneManager(const cSceneManager& obj) = delete;
public:
	static cSceneManager* GetInstance()
	{
		if (singleton == NULL)
		{
			singleton = new cSceneManager();
		}

		return singleton;
	}
	static void DestroyInstance()
	{
		if (singleton != NULL)
		{
			delete singleton;
			singleton = NULL;
		}
	}

private:
	std::vector<Pokemon::SpawnData> loadedSpawnData;
};