#include "cWeatherManager.h"
#include "cFloatAnimation.h"
#include "cParticleManager.h"
#include "cRenderManager.h"

cWeatherManager* cWeatherManager::singleton = NULL;

cWeatherManager::cWeatherManager()
{
	currWeather = NONE;

	fogColor = glm::vec3(255.f);
	fogGradient = 0.001f;
}

cWeatherManager::~cWeatherManager()
{
	
}

void cWeatherManager::SetWeather(eWeather newWeather)
{
	if (newWeather == currWeather)
		return;

	if ((currWeather == SNOW || currWeather == HAIL || currWeather == SNOWSTORM) // snow transition
		&& (newWeather == SNOW || newWeather == HAIL || newWeather == SNOWSTORM))
	{
		// animate values to correct number
		

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
		// animate values to correct number


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

			std::shared_ptr<cRenderModel> prtcl = cRenderManager::CreateRenderModel();
			prtcl->meshName = "SpriteHolder.obj";
			prtcl->shaderName = "particle";
			prtcl->textureName = "SnowFlake3.png";
			prtcl->scale = glm::vec3(0.3f);

			cParticleSpawner weatherSpawner(glm::vec3(0.f, 20.f, 0.f), prtcl, 1000);
			weatherSpawner.minPositionOffset = glm::vec3(-20.f, 0.f, -20.f);
			weatherSpawner.maxPositionOffset = glm::vec3(20.f, 0.f, 20.f);
			weatherSpawner.spawnSpeed = glm::vec3(0.f, -3.f, 0.f);
			weatherSpawner.spawnRate = 0.05f;
			weatherSpawner.particleLifeTime = 10.f;
			cParticleManager::GetInstance()->AddSpawner(weatherSpawner);
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