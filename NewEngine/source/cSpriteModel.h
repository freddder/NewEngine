#pragma once

#include "cRenderModel.h"

class cSpriteModel : public cRenderModel
{
public:

	cSpriteModel();

	int currSpriteId;

	virtual void SetUpUniforms();
};