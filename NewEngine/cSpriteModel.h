#pragma once

#include "cRenderModel.h"

class cSpriteModel : public cRenderModel
{
public:

	cSpriteModel();

	int currSpriteId;

	void SetUpUniforms();
};