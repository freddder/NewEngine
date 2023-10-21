#pragma once
#include <glm/glm.hpp>
#include <vector>

struct sLight
{
	glm::vec4 position;
	glm::vec4 diffuse;
	glm::vec4 direction;	// Spot, directional lights
	//glm::vec4 specular;		// rgb = highlight colour, w = power
	//glm::vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	glm::vec4 extraParam;   // x = lightType, y = inner angle, z = outer angle, w = is on (0 = off, 1 = on)
					// 0 = pointlight
					// 1 = spot light
					// 2 = directional light

	sLight()
	{
		position = glm::vec4(0.f, 50.f, 0.f, 1.f);
		diffuse = glm::vec4(1.f, 1.f, 1.f, 1.f);
		direction = glm::vec4(0.f, -1.f, 0.f, 1.f);
		extraParam = glm::vec4(0.f, glm::radians(30.f), glm::radians(30.f), 0.f);
	};
};

class cLightManager
{
	static cLightManager* singleton;
	cLightManager();
	~cLightManager();
	cLightManager(const cLightManager& obj) = delete;
public:
	static cLightManager* GetInstance()
	{
		if (singleton == NULL)
		{
			singleton = new cLightManager();
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

	const static unsigned int NUMBER_OF_LIGHTS = 20;
	sLight lights[NUMBER_OF_LIGHTS];

	int shadowSampleRadius;

	void AddProgramToBlock(unsigned int newProgram); // called everytime a new program is created
	void SetUnimormValues(); // called every frame

private:
	unsigned int uboLights;
};