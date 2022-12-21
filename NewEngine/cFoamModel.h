#pragma once
#include "cRenderModel.h"

class cFoamModel : public cRenderModel
{
public:

	glm::vec3 textureOffset;

	cFoamModel();

	virtual void SetUpUniforms();
};