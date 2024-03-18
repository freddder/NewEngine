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
	LEAVES,
	ENUM_COUNT
};

const static char* Weather_Strings[] =
{
	"None",
	"Snow",
	"Hail",
	"Snowstorm",
	"Rain",
	"Heavy Rain",
	"Sandstorm",
	"Leaves"
};

class cSceneManager
{
	// Singleton
	static cSceneManager* singleton;
	cSceneManager();
	~cSceneManager();
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
	cParticleSpawner* weatherParticleSpawner;
	float fogDensity;
	float fogGradient;
	glm::vec3 fogColor;
	float windSpeed;
	void SetWeather(eEnvironmentWeather newWeather);

	// Wild pokemon
private:
	std::vector<Pokemon::SpawnData> loadedSpawnData;
	//std::vector<cOverworldPokemon> roamingWildPokemon;
public:
	void SpawnWildPokemon();

public:
	void ChangeScene();
	void Process(float deltaTime);
};