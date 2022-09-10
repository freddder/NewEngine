#include "cWeatherManager.h"
#include "cLinearCongruentialGenerator.h"
#include "Global.h"
#include <ctime>
#include <glm/gtc/type_ptr.hpp>

cWeatherManager::cWeatherManager()
{
	currWeather = SNOW;
	fogDensity = 0.f;
	fogColor = glm::vec3(0);

	offsetDegree = 0.f;

	std::vector<double> random_draws;

	sWeatherParticleType snow1;
	sWeatherParticleType snow2;
	sWeatherParticleType snow3;

	snow1.textureName = "SnowFlake1.png";
	snow1.modelName = "SpriteHolder2.obj";

	snow2.textureName = "SnowFlake2.png";
	snow2.modelName = "SpriteHolder2.obj";

	snow3.textureName = "SnowFlake3.png";
	snow3.modelName = "SpriteHolder2.obj";

	unsigned int numSnow1 = 4;
	unsigned int numSnow2 = 6;
	unsigned int numSnow3 = 8;

	glGenBuffers(1, &snow1.positionsBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, snow1.positionsBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * numSnow1,	NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &snow2.positionsBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, snow2.positionsBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * numSnow2, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &snow3.positionsBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, snow3.positionsBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * numSnow3, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	srand((int)time(0));
	unsigned int init_seed = (rand() % 10) + 1;

	cLinearCongruentialGenerator lcg(init_seed);
	lcg.get_uniform_draws2(random_draws, numSnow1);

	for (unsigned int i = 0; i < random_draws.size(); i++)
	{
		// speed = 0.0714 - 0.1666

		sWeatherParticlePosition newPosition;
		newPosition.position.x = random_draws[i];
		newPosition.position.y = (float)(rand() % 200 - 100) / (float)100;
		newPosition.speed.x = 0.f;
		newPosition.speed.y = (float)(rand() % 10 + 10) / (float)100 * -1;

		snow1.positions.push_back(newPosition);
	}
	
	lcg.set_random_seed((rand() % 10) + 1);
	random_draws.clear();
	lcg.get_uniform_draws2(random_draws, numSnow2);

	for (unsigned int i = 0; i < random_draws.size(); i++)
	{
		// speed = 0.1 - 0.2

		sWeatherParticlePosition newPosition;
		newPosition.position.x = random_draws[i];
		newPosition.position.y = (float)(rand() % 200 - 100) / (float)100;
		newPosition.speed.x = 0.f;
		newPosition.speed.y = (float)(rand() % 10 + 10) / (float)100 * -1;

		snow2.positions.push_back(newPosition);
	}

	lcg.set_random_seed((rand() % 10) + 1);
	random_draws.clear();
	lcg.get_uniform_draws2(random_draws, numSnow3);

	for (unsigned int i = 0; i < random_draws.size(); i++)
	{
		// speed = 0.0714 - 0.1666

		sWeatherParticlePosition newPosition;
		newPosition.position.x = random_draws[i];
		newPosition.position.y = (float)(rand() % 200 - 100) / (float)100;
		newPosition.speed.x = 0.f;
		newPosition.speed.y = (float)(rand() % 10 + 10) / (float)100 * -1;

		snow3.positions.push_back(newPosition);
	}

	particleTypes.push_back(snow1);
	particleTypes.push_back(snow2);
	particleTypes.push_back(snow3);
}

cWeatherManager::~cWeatherManager()
{
	for (unsigned int i = 0; i < particleTypes.size(); i++)
	{
		glDeleteBuffers(1, &particleTypes[i].positionsBufferId);
	}
}

void cWeatherManager::Process(float deltaTime)
{
	for (unsigned int i = 0; i < particleTypes.size(); i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, particleTypes[i].positionsBufferId);

		for (unsigned j = 0; j < particleTypes[i].positions.size(); j++)
		{
			particleTypes[i].positions[j].position += particleTypes[i].positions[j].speed * deltaTime;

			if (particleTypes[i].positions[j].position.y <= -1.2f)
			{
				particleTypes[i].positions[j].position.y = 1.2f;
			}

			glBufferSubData(GL_ARRAY_BUFFER, j * sizeof(glm::vec2), sizeof(glm::vec2), glm::value_ptr(particleTypes[i].positions[j].position));
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// move UVs here i guess
}
