#pragma once
#include "cRenderModel.h"

class cOceanModel : public cRenderModel
{
public:

	glm::vec2 globalUVRatios;
	glm::vec3 textureOffset;

	cOceanModel();

	virtual void SetUpUniforms();
};