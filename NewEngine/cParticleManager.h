#pragma once
#include <vector>
#include "cParticleSpawner.h"

class cParticleManager 
{
	std::vector<cParticleSpawner> particleSpawners;

public:

	cParticleManager();

	void AddSpawner(cParticleSpawner newSpawner);
	void UpdateSpawners(float deltaTime);
	void DrawSpawnerParticles();
};