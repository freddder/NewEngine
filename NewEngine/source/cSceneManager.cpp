#include "cSceneManager.h"
#include "cWildRoamingPokemon.h"
#include "cMapManager.h"
#include "cRenderManager.h"

#include <time.h>

cSceneManager* cSceneManager::sgtn = NULL;

cSceneManager::cSceneManager()
{
	currWeather = NONE;

	fogDensity = 0.f;
	fogGradient = 0.1f;
	windSpeed = 0.25f;

}

cSceneManager::~cSceneManager()
{
	for (int i = 0; i < particleSpawners.size(); i++)
	{
		delete particleSpawners[i];
	}

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
	cRenderManager* renderManager = cRenderManager::GetInstance();
	renderManager->LoadOverworldPokemonSpriteSheet(nationalDexId, formName);

	// Load female varient if there is one
	if (formName == "" && (specieData.isSpriteGenderBased || specieData.isFormGenderBased))
	{
		renderManager->LoadOverworldPokemonSpriteSheet(nationalDexId, "f");
	}

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
	Pokemon::sSpawnData spawnData = loadedSpawnData[(rand() % loadedSpawnData.size() - 1)];

	// Find a suitable tile
	if (spawnData.spawnType == Pokemon::TALL_GRASS)
	{

	}
	
	return std::shared_ptr<cWildRoamingPokemon>();
}

std::shared_ptr<cWildRoamingPokemon> cSceneManager::SpawnWildPokemon(const Pokemon::sSpawnData& spawnData, glm::vec3 location)
{
	// Check if location is available
	sTile* tile = cMapManager::GetInstance()->GetTile(location);
	if (!tile) return nullptr;

	// Determine gender
	Pokemon::eGender gender = Pokemon::NO_GENDER;
	if (spawnData.genderRatio >= 0)
	{		
		int genderRandom = (rand() % 100); // [0-99]

		if (spawnData.genderRatio < genderRandom) gender = Pokemon::MALE;
		else gender = Pokemon::FEMALE;
	}

	std::string textureName = std::to_string(spawnData.nationalDexNumber);
	if (gender == Pokemon::FEMALE && (spawnData.isSpriteGenderBased || spawnData.isFormGenderBased))
	{
		textureName = textureName + "_f";
	}
	else if (spawnData.formName != "")
	{
		textureName = textureName + "_" + spawnData.formName;
	}
	
	// Determine shiny
	int shinyRandom = (rand() % 100); // [0-99]
	if (shinyRandom < 50) textureName = textureName + "_s";

	textureName = textureName + ".png";
	
	std::shared_ptr<cWildRoamingPokemon> newWildPokemon = std::make_shared<cWildRoamingPokemon>(location, textureName);
	roamingWildPokemon.push_back(newWildPokemon);

	tile->entity = newWildPokemon.get();

	return newWildPokemon;
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
