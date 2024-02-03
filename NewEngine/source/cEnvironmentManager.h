#pragma once
#include <vector>
#include "cParticleSpawner.h"

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

class cEnvironmentManager
{
	static cEnvironmentManager* singleton;
	cEnvironmentManager();
	~cEnvironmentManager();
	cEnvironmentManager(const cEnvironmentManager& obj) = delete;
public:
	static cEnvironmentManager* GetInstance()
	{
		if (singleton == NULL)
		{
			singleton = new cEnvironmentManager();
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
	void SetWeather(eEnvironmentWeather newWeather);

public:
	float fogDensity;
	float fogGradient;
	glm::vec3 fogColor;

	std::vector<cParticleSpawner*> particleSpawners;

public:
	void Process(float deltaTime);
};