#pragma once
#include "cAnimatedModel.h"
#include "cSinAnimation.h"

class cOceanModel : public cAnimatedModel
{
public:

	glm::vec2 globalUVRatios;
	glm::vec3 textureOffset;
	float timer;

	cOceanModel();
	~cOceanModel();

	virtual void SetUpUniforms();
};