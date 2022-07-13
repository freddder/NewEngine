#pragma once

#include <glm/glm.hpp>
#include <string>

struct sKeyFrameVec3 // rgb
{
	glm::vec3 value;
	float time;
};

struct sKeyFrame3Vec3 // position, rotation, scale
{
	glm::vec3 value1;
	glm::vec3 value2;
	glm::vec3 value3;
	float time;
};

struct sKeyFrameString // texture
{
	std::string value;
	float time;
};

struct sKeyFrameFloat // who knows
{
	float value;
	float time;
};

struct sKeyFrameInt // sprite
{
	unsigned int value;
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

	void Reset()
	{
		timer = 0.f;
		isDone = false;
	}
};