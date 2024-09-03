#include "cSceneManager.h"
#include "cWildRoamingPokemon.h"
#include "cTamedRoamingPokemon.h"
#include "Player.h"

#include <time.h>

#include "Engine.h"
#include "cMapManager.h"
#include "cRenderManager.h"
#include "cInputManager.h"

cSceneManager::cSceneManager()
{
	currWeather = NONE;

	fogDensity = 0.f;
	fogGradient = 0.1f;
	windSpeed = 0.25f;
}

cSceneManager::~cSceneManager()
{
}

void cSceneManager::Shutdown()
{
	for (int i = 0; i < particleSpawners.size(); i++)
	{
		delete particleSpawners[i];
	}
	particleSpawners.clear();

	roamingWildPokemon.clear();
}

void cSceneManager::SetWeather(eEnvironmentWeather newWeather)
{
	if (newWeather == currWeather) return;

	if ((currWeather == SNOW || currWeather == HAIL || currWeather == SNOWSTORM) // snow transition
		&& (newWeather == SNOW || newWeather == HAIL || newWeather == SNOWSTORM))
	{
		if (newWeather == SNOW)
		{

		}
		else if (newWeather == HAIL)
		{

		}
	}
	else if ((currWeather == RAIN || currWeather == HEAVYRAIN) // rain transition
		&& (newWeather == RAIN || newWeather == HEAVYRAIN))
	{



	}
	else // no transition
	{
		if (weatherParticleSpawner)
		{
			delete weatherParticleSpawner;
			weatherParticleSpawner = nullptr;
		}

		if (newWeather == NONE)
		{
			fogDensity = 0.f;
			fogGradient = 0.1f;
		}
		else if (newWeather == SNOW)
		{
			fogDensity = 0.057f;
			fogGradient = 0.57f;
			fogColor = glm::vec3(0.89f, 0.89f, 0.89f);

			cRenderModel prtcl;
			prtcl.meshName = "SpriteHolder.obj";
			prtcl.shaderName = "snow";
			prtcl.textureName = "SnowFlake3.png";
			prtcl.scale = glm::vec3(0.3f);

			weatherParticleSpawner = new cParticleSpawner(glm::vec3(0.f, 13.f, 0.f), prtcl, 500);
			weatherParticleSpawner->minPositionOffset = glm::vec3(-20.f, 0.f, -20.f);
			weatherParticleSpawner->maxPositionOffset = glm::vec3(20.f, 0.f, 20.f);
			weatherParticleSpawner->isPositionPlayerRelative = true;
			weatherParticleSpawner->spawnSpeed = glm::vec3(0.f, -3.f, 0.f);
			weatherParticleSpawner->spawnRate = 0.05f;
			weatherParticleSpawner->particleLifeTime = 10.f;
		}
		else if (newWeather == HAIL)
		{
			fogDensity = 0.022f;
			fogGradient = 0.48f;
			fogColor = glm::vec3(0.89f, 0.89f, 0.89f);


		}
		else if (newWeather == SNOWSTORM)
		{

		}
		else if (newWeather == RAIN)
		{

		}
		else if (newWeather == HEAVYRAIN)
		{

		}
		else if (newWeather == SANDSTORM)
		{

		}
		else if (newWeather == LEAVES)
		{

		}
	}

	currWeather = newWeather;
}

void cSceneManager::LoadSpawnData(const int nationalDexId, const int minLevel, const int maxLevel, const Pokemon::eSpawnType spawnType, const int spawnChance, const std::string formName)
{
	for (int i = 0; i < loadedSpawnData.size(); i++)
	{
		if (loadedSpawnData[i].nationalDexNumber == nationalDexId && loadedSpawnData[i].formName == formName)
		{
			return; // Already loaded
		}
	}

	Pokemon::sSpeciesData specieData;
	Pokemon::LoadSpecieData(nationalDexId, specieData);

	// Load overworld sprite texture
	Manager::render.LoadRoamingPokemonSpecieTextures(specieData);

	Pokemon::sSpawnData spawnData;
	spawnData.nationalDexNumber = nationalDexId;
	spawnData.formName = formName;
	spawnData.minLevel = minLevel;
	spawnData.maxLevel = maxLevel;
	spawnData.spawnType = spawnType;
	spawnData.spawnChance = spawnChance;
	spawnData.genderRatio = specieData.genderRatio;
	spawnData.isFormGenderBased = specieData.isFormGenderBased;
	spawnData.isSpriteGenderBased = specieData.isSpriteGenderBased;

	loadedSpawnData.push_back(spawnData);
}

std::shared_ptr<cWildRoamingPokemon> cSceneManager::SpawnRandomWildPokemon()
{
	if (loadedSpawnData.size() == 0) return nullptr;

	// Pick a random spawn data
	int randIndex = rand() % loadedSpawnData.size();
	Pokemon::sSpawnData spawnData = loadedSpawnData[randIndex];
	std::shared_ptr<cWildRoamingPokemon> spawnedWildPokemon = nullptr;

	// Find a suitable tile
	if (spawnData.spawnType == Pokemon::TALL_GRASS)
	{
		glm::vec3 spawnTilePos;
		sTile* spawnTile = Manager::map.GetRandomSpawnTile(spawnTilePos);

		if (spawnTile)
			spawnedWildPokemon = SpawnWildPokemon(spawnData, spawnTilePos, spawnTile);
	}
	
	return spawnedWildPokemon;
}

std::shared_ptr<cWildRoamingPokemon> cSceneManager::SpawnWildPokemon(const Pokemon::sSpawnData& spawnData, glm::vec3 tileLocation, sTile* spawnTile)
{
	if (!spawnTile) return nullptr;

	Pokemon::sRoamingPokemonData roamingData = Pokemon::GenerateRoamingPokemonData(spawnData);

	std::shared_ptr<cWildRoamingPokemon> newWildPokemon = std::make_shared<cWildRoamingPokemon>(roamingData, tileLocation);
	roamingWildPokemon.push_back(newWildPokemon);

	spawnTile->entity = newWildPokemon.get();

	return newWildPokemon;
}

std::shared_ptr<cTamedRoamingPokemon> cSceneManager::SpawnTamedPokemon(Pokemon::sRoamingPokemonData& pokemonData, glm::vec3 tileLocation)
{
	sTile* spawnTile = Manager::map.GetTile(tileLocation);
	if (!spawnTile || spawnTile->entity != nullptr) return nullptr;

	std::shared_ptr<cTamedRoamingPokemon> newTamedPokemon = std::make_shared<cTamedRoamingPokemon>(pokemonData, tileLocation);
	roamingTamedPokemon.push_back(newTamedPokemon);

	spawnTile->entity = newTamedPokemon.get();

	return newTamedPokemon;
}

void cSceneManager::ChangeScene()
{
	// Things this should do (not ordered):
	// - unload current map model
	// - unload map textures
	// - unload map data
	// - despawn entities
	// - unload despawn data
	// - move player and follower to appropriate place
	// - remove render objects from vector
}

void cSceneManager::EnterWildEncounter(const Pokemon::sRoamingPokemonData& roamingPokemonData)
{
	Pokemon::sBattleData battleData = Pokemon::GeneratePokemonBattleData(roamingPokemonData);
	std::string texName = battleData.MakeBattleTextureName();

	// TODO: create new canvas for the battle
	// Canvas should probably have a confirm and cancel function that changes what it does
	// depending of situations

	Manager::render.ChangeRenderMode(BATTLE);
	Manager::input.ChangeInputState(MENU_NAVIGATION);

	float spriteAspectRatio = Manager::render.LoadPokemonBattleSpriteSheet(battleData);
	Manager::map.opponentSpriteModel->SetSpriteData(texName, battleData.form.battleSpriteHeightSize, spriteAspectRatio, battleData.form.battleSpriteFrameCount);
}

void cSceneManager::RunEncounter()
{
	Manager::render.ChangeRenderMode(MAP);
	Manager::input.ChangeInputState(OVERWORLD_MOVEMENT);

	Manager::map.opponentSpriteModel->ClearSpriteData();
}

void cSceneManager::Process(float deltaTime)
{
	if (weatherParticleSpawner)
	{
		weatherParticleSpawner->Update(deltaTime);
	}

	for (int i = 0; i < particleSpawners.size(); i++)
	{
		particleSpawners[i]->Update(deltaTime);
	}
}
