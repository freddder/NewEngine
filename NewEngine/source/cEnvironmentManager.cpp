#include "cEnvironmentManager.h"
#include "cFloatAnimation.h"
#include "cRenderManager.h"

cEnvironmentManager* cEnvironmentManager::singleton = NULL;

cEnvironmentManager::cEnvironmentManager()
{
	currWeather = NONE;

	fogDensity = 0.f;
	fogGradient = 0.1f;
}

cEnvironmentManager::~cEnvironmentManager()
{
	
}

void cEnvironmentManager::SetWeather(eEnvironmentWeather newWeather)
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
		if (newWeather == NONE)
		{
			fogDensity = 0.f;
			fogGradient = 0.1f;
		}
		else if (newWeather == SNOW)
		{
			fogDensity = 0.037f;
			fogGradient = 1.59f;
			fogColor = glm::vec3(0.89f, 0.89f, 0.89f);

			cRenderModel prtcl;
			prtcl.meshName = "SpriteHolder.obj";
			prtcl.shaderName = "snow";
			prtcl.textureName = "SnowFlake3.png";
			prtcl.scale = glm::vec3(0.3f);

			cParticleSpawner* weatherSpawner = new cParticleSpawner(glm::vec3(0.f, 20.f, 0.f), prtcl, 500);
			weatherSpawner->minPositionOffset = glm::vec3(-20.f, 0.f, -20.f);
			weatherSpawner->maxPositionOffset = glm::vec3(20.f, 0.f, 20.f);
			weatherSpawner->spawnSpeed = glm::vec3(0.f, -3.f, 0.f);
			weatherSpawner->spawnRate = 0.05f;
			weatherSpawner->particleLifeTime = 10.f;
			particleSpawners.push_back(weatherSpawner);
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

void cEnvironmentManager::Process(float deltaTime)
{
	for (int i = 0; i < particleSpawners.size(); i++)
	{
		particleSpawners[i]->Update(deltaTime);
	}
}
