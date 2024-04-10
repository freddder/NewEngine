#include "cParticleSpawner.h"

#include "Player.h"

#include "cLinearCongruentialGenerator.h"
#include <time.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

cParticleSpawner::cParticleSpawner(glm::vec3 position, cRenderModel _model, int _maxParticles)// : lcgX(0), lcgY(0), lcgZ(0)
{
	spawnPosition = position;
	minPositionOffset = glm::vec3(0.f);
	maxPositionOffset = glm::vec3(0.f);
	isPositionPlayerRelative = false;

	spawnSpeed = glm::vec3(0.f, -1.f, 0.f);
	minSpeedOffset = glm::vec3(0.f);
	maxSpeedOffset = glm::vec3(0.f);
	gravity = glm::vec3(0.f);

	particles.reserve(_maxParticles);
	maxParticles = _maxParticles; // if -1, no limit
	spawnRate = 0.1f; // particles spawned per second
	timer = 0.f;

	model = _model;

	// do the random
	srand((int)time(0));
	unsigned int init_seed = (rand() % 100) + 1;
	lcgX = cLinearCongruentialGenerator(init_seed);

	init_seed = (rand() % 100) + 1;
	lcgY = cLinearCongruentialGenerator(init_seed);

	init_seed = (rand() % 100) + 1;
	lcgZ = cLinearCongruentialGenerator(init_seed);

	// Create buffer for particle data (position + timer)
	glGenBuffers(1, &(particleBufferId));
	glBindBuffer(GL_ARRAY_BUFFER, particleBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * _maxParticles, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

cParticleSpawner::~cParticleSpawner()
{
	particles.clear();
}

void cParticleSpawner::Update(float deltaTime)
{
	timer += deltaTime;

	if (timer > spawnRate && particles.size() < particles.capacity())
	{
		// add new particle
		double randomPosX;
		double randomPosY;
		double randomPosZ;

		lcgX.get_uniform_draw(randomPosX);
		lcgY.get_uniform_draw(randomPosY);
		lcgZ.get_uniform_draw(randomPosZ);

		//NewValue = (((OldValue - OldMin) * (NewMax - NewMin)) / (OldMax - OldMin)) + NewMin
		glm::vec3 newParticlePosOffset = glm::vec3(
			(((float)randomPosX * (maxPositionOffset.x - minPositionOffset.x)) / (1 - 0)) + minPositionOffset.x,
			(((float)randomPosY * (maxPositionOffset.y - minPositionOffset.y)) / (1 - 0)) + minPositionOffset.y,
			(((float)randomPosZ * (maxPositionOffset.z - minPositionOffset.z)) / (1 - 0)) + minPositionOffset.z 
		);

		// add speed offset as well

		sParticle newParticle;
		newParticle.position = newParticlePosOffset + spawnPosition;
		newParticle.speed = spawnSpeed;
		newParticle.timer = 0.f;

		if (isPositionPlayerRelative)
		{
			newParticle.position += Player::GetPlayerPosition();
		}

		particles.push_back(newParticle);

		timer -= spawnRate;
	}

	for (int i = 0; i < particles.size(); i++)
	{
		particles[i].timer += deltaTime;

		if (particles[i].timer > particleLifeTime)
		{
			particles.erase(particles.begin() + i);
			i--;

			continue;
		}

		// update position
		particles[i].position += particles[i].speed * deltaTime;

		// update speed for gravity stuff		

		// update buffer
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * i, sizeof(glm::vec4), glm::value_ptr(glm::vec4(particles[i].position, particles[i].timer)));
	}
}
