#pragma once

#include "cAnimation.h"

class cModelAnimation : public cAnimation
{
	glm::vec3& positionRef;
	glm::vec3& orintationRef;
	glm::vec3& scaleRef;
};