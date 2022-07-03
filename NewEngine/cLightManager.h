#pragma once
#include <glm/glm.hpp>

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

	int position_uniform_location;
	int diffuse_uniform_location;
	int direction_uniform_location;
	//int specular_uniform_location;
	//int atten_uniform_location;
	int extraParam_uniform_location;

	sLight()
	{
		position = glm::vec4(0.f, 50.f, 0.f, 1.f);
		diffuse = glm::vec4(1.f, 1.f, 1.f, 1.f);
		direction = glm::vec4(0.f, -1.f, 0.f, 1.f);
		extraParam = glm::vec4(0.f, glm::radians(30.f), glm::radians(30.f), 0.f);

		position_uniform_location = -1;
		diffuse_uniform_location = -1;
		direction_uniform_location = -1;
		//specular_uniform_location = -1;
		//atten_uniform_location = -1;
		extraParam_uniform_location = -1;
	};
};

class cLightManager
{
public:

	const static unsigned int NUMBER_OF_LIGHTS = 20;
	sLight lights[NUMBER_OF_LIGHTS];

	void SetupUniformLocations(unsigned int program); // called once
	void SetUnimormValues(unsigned int program); // called every frame
};