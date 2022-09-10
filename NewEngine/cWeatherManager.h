#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
//#include "cRenderModel.h"

enum eWeather
{
	SNOW,
	HAIL,
	SNOWSTORM,
	RAIN,
	SANDSTORM
};

struct sWeatherParticlePosition
{
	glm::vec2 position;
	glm::vec2 speed;
};

struct sWeatherParticleType
{
	std::string textureName;
	std::string modelName;

	std::vector<sWeatherParticlePosition> positions;
	unsigned int positionsBufferId;
};

class cWeatherManager
{
	eWeather currWeather;

	//glm::vec2 onScreenSpeed;

	float fogDensity;
	glm::vec3 fogColor;

public:

	std::vector<sWeatherParticleType> particleTypes;

	float offsetDegree;

	cWeatherManager();
	~cWeatherManager();

	void SetWeather(eWeather newWeather);

	void Process(float deltaTime);
};