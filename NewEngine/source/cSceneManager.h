#pragma once

// purpose: load necessary textures and 3d models and deal with rendering
#include <string>
#include "cParticleSpawner.h"
#include "PokemonData.h"

class cWildRoamingPokemon;
class cTamedRoamingPokemon;
struct sTile;

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
	static cSceneManager* sgtn;
	cSceneManager();
	~cSceneManager();
	cSceneManager(const cSceneManager& obj) = delete;
public:
	static cSceneManager* GetInstance()
	{
		if (sgtn == NULL)
		{
			sgtn = new cSceneManager();
		}

		return sgtn;
	}
	static void DestroyInstance()
	{
		if (sgtn != NULL)
		{
			delete sgtn;
			sgtn = NULL;
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

	// Entities
private:
	std::vector<Pokemon::sSpawnData> loadedSpawnData;
	std::vector<std::shared_ptr<cWildRoamingPokemon>> roamingWildPokemon;
	std::vector<std::shared_ptr<cTamedRoamingPokemon>> roamingTamedPokemon;
public:
	void LoadSpawnData(const int nationalDexId, const int minLevel, const int maxLevel, const Pokemon::eSpawnType spawnType,  const int spawnChance, const std::string formName = "");
	std::shared_ptr<cWildRoamingPokemon> SpawnRandomWildPokemon();
	std::shared_ptr<cWildRoamingPokemon> SpawnWildPokemon(Pokemon::sSpawnData& spawnData, glm::vec3 tileLocation, sTile* spawnTile);
	std::shared_ptr<cTamedRoamingPokemon> SpawnTamedPokemon(Pokemon::sPokemonData& pokemonData, glm::vec3 tileLocation);

public:
	void ChangeScene();
	void Process(float deltaTime);
};