#pragma once
#include "cAnimatedModel.h"

class cFoamModel : public cAnimatedModel
{
public:

	glm::vec3 textureOffset;

	cFoamModel();
	~cFoamModel();

	virtual void SetUpUniforms();
};