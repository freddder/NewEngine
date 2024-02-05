#pragma once

// purpose: load necessary textures and 3d models and deal with rendering
#include <string>
#include "cParticleSpawner.h"
#include "PokemonData.h"

class cOverworldPokemon;

enum eEnvironmentWeather
{
	NONE,
	SNOW,
	HAIL,
	SNOWSTORM,
	RAIN,
	HEAVYRAIN,
	SANDSTORM,
	LEAVES
};

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

	// Envoirnment weather
private:
	eEnvironmentWeather currWeather;
public:
	std::vector<cParticleSpawner*> particleSpawners;
	float fogDensity;
	float fogGradient;
	glm::vec3 fogColor;
	void SetWeather(eEnvironmentWeather newWeather);

private:
	std::vector<Pokemon::SpawnData> loadedSpawnData;
	//std::vector<cOverworldPokemon> roamingWildPokemon;
public:
	void SpawnWildPokemon();

public:
	void Process(float deltaTime);
};