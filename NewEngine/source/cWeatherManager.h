#pragma once
#include <vector>
#include "cParticleSpawner.h"
//#include "cRenderModel.h"

enum eWeather
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

class cWeatherManager
{
	static cWeatherManager* singleton;
	cWeatherManager();
	~cWeatherManager();
	cWeatherManager(const cWeatherManager& obj) = delete;
public:
	static cWeatherManager* GetInstance()
	{
		if (singleton == NULL)
		{
			singleton = new cWeatherManager();
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
	eWeather currWeather;

public:
	float fogDensity;
	float fogGradient;
	glm::vec3 fogColor;

	std::vector<cParticleSpawner> particleSpawners;

	void SetWeather(eWeather newWeather);
};