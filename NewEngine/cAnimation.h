#pragma once

#include <glm/glm.hpp>
#include <string>

struct sVec3KeyFrame // rgb
{
	glm::vec3 value;
	float time;
};

struct s3Vec3KeyFrame // position, rotation, scale
{
	glm::vec3 value1;
	glm::vec3 value2;
	glm::vec3 value3;
	float time;
};

struct sStringKeyFrame // texture
{
	std::string value;
	float time;
};

struct sFloatKeyFrame // who knows
{
	float value;
	float time;
};

class cAnimation
{
public:

	float timer;
	bool isDone;
	float maxDuration;
	float speed;
	bool isRepeat;

	cAnimation()
	{
		timer = 0.f;
		isDone = false;
		maxDuration = 0.f;
		speed = 1.f;
		isRepeat = false;
	}

	virtual void Process(float deltaTime) = 0;
};