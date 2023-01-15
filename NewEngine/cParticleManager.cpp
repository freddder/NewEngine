#include "cParticleManager.h"

cParticleManager* cParticleManager::singleton = NULL;

cParticleManager::cParticleManager()
{

}

void cParticleManager::AddSpawner(cParticleSpawner newSpawner)
{
	particleSpawners.push_back(newSpawner);
}

void cParticleManager::UpdateSpawners(float deltaTime)
{
	for (int i = 0; i < particleSpawners.size(); i++)
	{
		particleSpawners[i].Update(deltaTime);
	}
}

void cParticleManager::DrawSpawnerParticles()
{
	for (int i = 0; i < particleSpawners.size(); i++)
	{
		particleSpawners[i].DrawParticles();
	}
}