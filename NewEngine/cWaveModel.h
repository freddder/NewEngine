#pragma once
#include "cRenderModel.h"

class cWaveModel : public cRenderModel
{
public:

	glm::vec3 textureOffset;

	cWaveModel();

	virtual void SetUpUniforms();
};