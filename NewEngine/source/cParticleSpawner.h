#pragma once
#include "cRenderModel.h"
#include "cLinearCongruentialGenerator.h"
#include <memory>

struct sParticle
{
	glm::vec3 position;
	glm::vec3 speed;
	float timer;
};

class cParticleSpawner
{
public:

	// Spawn position
	glm::vec3 spawnPosition;
	glm::vec3 minPositionOffset;
	glm::vec3 maxPositionOffset;
	bool isPositionPlayerRelative;

	// Spawn speed
	glm::vec3 spawnSpeed;
	glm::vec3 minSpeedOffset;
	glm::vec3 maxSpeedOffset;
	glm::vec3 gravity;

	int maxParticles; // if -1, no limit
	float spawnRate; // particles spawned per second
	float timer;
	float particleLifeTime;

	// Model
	cRenderModel model;
	unsigned int particleBufferId;

	std::vector<sParticle> particles;

	// Randomizers
	cLinearCongruentialGenerator lcgX;
	cLinearCongruentialGenerator lcgY;
	cLinearCongruentialGenerator lcgZ;

	cParticleSpawner(glm::vec3 position, cRenderModel _model, int _maxParticles);
	~cParticleSpawner();

	void Update(float deltaTime);

	//void DrawParticles();
};