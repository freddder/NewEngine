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
	cParticleSpawner(glm::vec3 position, cRenderModel _model, unsigned int _maxParticles);
	~cParticleSpawner();

	// Spawn position
private:
	cLinearCongruentialGenerator lcgPosX;
	cLinearCongruentialGenerator lcgPosY;
	cLinearCongruentialGenerator lcgPosZ;
public:
	glm::vec3 spawnPosition;
	glm::vec3 minPositionOffset = glm::vec3(0.f);
	glm::vec3 maxPositionOffset = glm::vec3(0.f);
	bool isPositionPlayerRelative = false;

	// Spawn speed
private:
	cLinearCongruentialGenerator lcgSpdX;
	cLinearCongruentialGenerator lcgSpdY;
	cLinearCongruentialGenerator lcgSpdZ;
public:
	glm::vec3 spawnSpeed = glm::vec3(0.f);
	glm::vec3 minSpeedOffset = glm::vec3(0.f);
	glm::vec3 maxSpeedOffset = glm::vec3(0.f);
	glm::vec3 gravity = glm::vec3(0.f);

private:
	std::vector<sParticle> particles;
	float timer = 0.f;
	int maxParticles;
public:
	float spawnRate = -1.f; // particles spawned per second; negative for not spawn on timer
	float particleLifeTime = 1.f;

private:
	// Model
	cRenderModel model;
	unsigned int particleBufferId = 0;

private:
	bool SpawnParticle();
public:
	void SpawnParticles(unsigned int numToSpawn);

	void Update(float deltaTime);

	friend class cRenderManager;
};