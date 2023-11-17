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
	LEAVES
};

//struct sWeatherParticlePosition
//{
//	glm::vec2 position;
//	glm::vec2 speedOffset;
//};

//struct sWeatherParticleType
//{
//	std::string textureName;
//	std::string modelName;
//
//	std::vector<sWeatherParticlePosition> positions;
//	unsigned int positionsBufferId;
//};

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

	//std::vector<sWeatherParticleType> particleTypes;
	//glm::vec2 particleSpeed;
	//float offsetDegree;

	void SetWeather(eWeather newWeather);
};