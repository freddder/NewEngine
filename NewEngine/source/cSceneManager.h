#pragma once

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
public:
	cSceneManager();
	~cSceneManager();

	void Startup();
	void Shutdown();

	// Envoirnment weather
private:
	eEnvironmentWeather currWeather;
	cParticleSpawner* weatherParticleSpawner;
public:
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
	std::shared_ptr<cWildRoamingPokemon> SpawnWildPokemon(const Pokemon::sSpawnData& spawnData, glm::vec3 tileLocation, sTile* spawnTile);
	void DespawnWildPokemon(cWildRoamingPokemon* pokemonToDespawn);
	std::shared_ptr<cTamedRoamingPokemon> SpawnTamedPokemon(Pokemon::sRoamingPokemonData& pokemonData, glm::vec3 tileLocation);

private:
	Pokemon::sBattleData enemyBattleData;
public:
	void ChangeScene(const std::string newSceneDescFile, const int entranceNumUsed);
	void EnterWildEncounter(const Pokemon::sRoamingPokemonData& roamingPokemonData, cWildRoamingPokemon* roamingEntity);
	void CatchWildPokemon();
	void ExitEncounter();

private:
	std::vector<std::shared_ptr<cParticleSpawner>> particleSpawners;
public:
	std::shared_ptr<cParticleSpawner> CreateParticleSpawner(glm::vec3 position, cRenderModel model, unsigned int maxParticles);

	void Process(float deltaTime);

	friend class cRenderManager;
	friend class cEnemyBattleInfo;
};