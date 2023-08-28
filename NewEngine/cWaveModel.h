#pragma once
#include "cAnimatedModel.h"
#include "cSinAnimation.h"

class cWaveModel : public cAnimatedModel
{
public:

	glm::vec3 textureOffset;
	float timer;

	cWaveModel();
	~cWaveModel();

	virtual void SetUpUniforms();
};