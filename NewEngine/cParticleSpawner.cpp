#include "cParticleSpawner.h"
#include <glad/glad.h>
#include <time.h>
#include "cLinearCongruentialGenerator.h"
#include <glm/gtc/type_ptr.hpp>
#include "Global.h"

cParticleSpawner::cParticleSpawner(glm::vec3 position, cRenderModel* _model, int _maxParticles)// : lcgX(0), lcgY(0), lcgZ(0)
{
	spawnPosition = position;
	minPositionOffset = glm::vec3(0.f);
	maxPositionOffset = glm::vec3(0.f);

	spawnSpeed = glm::vec3(0.f, -1.f, 0.f);
	minSpeedOffset = glm::vec3(0.f);
	maxSpeedOffset = glm::vec3(0.f);
	gravity = glm::vec3(0.f);

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
	if(!model)
		delete model;
}

void cParticleSpawner::Update(float deltaTime)
{
	timer += deltaTime;

	if (timer > spawnRate)
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

		//update buffer
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * i, sizeof(glm::vec4), glm::value_ptr(glm::vec4(particles[i].position, particles[i].timer)));
	}
}

void cParticleSpawner::DrawParticles()
{
    sModelDrawInfo drawInfo;
    if (!g_ModelManager->FindModelByName(model->meshName, drawInfo))
        return;

    g_RenderManager->use("particle");

	g_RenderManager->setVec3("cameraPosition", g_Camera->position);
	g_RenderManager->setMat4("modelScale", glm::scale(glm::mat4(1.0f), model->scale));

	g_RenderManager->setBool("useWholeColor", model->useWholeColor);
	g_RenderManager->setVec4("wholeColor", model->wholeColor);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, g_RenderManager->GetDepthMapId());
	g_RenderManager->setInt("shadowMap", 1);

    for (unsigned int i = 0; i < drawInfo.allMeshesData.size(); i++)
    {
        // Setup texture
        std::string textureToUse = model->textureName;
        g_TextureManager->SetupTexture(textureToUse, 0);

        // Bind VAO
        glBindVertexArray(drawInfo.allMeshesData[i].VAO_ID);

		glBindBuffer(GL_ARRAY_BUFFER, particleBufferId);

		GLint offset_location = glGetAttribLocation(g_RenderManager->GetCurrentShaderId(), "oOffset");
		glEnableVertexAttribArray(offset_location);
		glVertexAttribPointer(offset_location, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
		glVertexAttribDivisor(offset_location, 1);

		glDrawElementsInstanced(GL_TRIANGLES,
			drawInfo.allMeshesData[i].numberOfIndices,
			GL_UNSIGNED_INT,
			(void*)0,
			particles.size());

        glBindVertexArray(0);
    }
}
