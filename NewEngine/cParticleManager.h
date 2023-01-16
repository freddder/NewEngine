#pragma once
#include <vector>
#include "cParticleSpawner.h"

class cParticleManager 
{
	static cParticleManager* singleton;
	cParticleManager();
	cParticleManager(const cParticleManager& obj) = delete;

	std::vector<cParticleSpawner> particleSpawners;

public:

    static cParticleManager* GetInstance()
    {
        if (singleton == NULL)
        {
            singleton = new cParticleManager();
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

	void AddSpawner(cParticleSpawner newSpawner);
	void UpdateSpawners(float deltaTime);
	void DrawSpawnerParticles();
};