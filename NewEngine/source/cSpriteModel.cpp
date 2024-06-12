#include "cSpriteModel.h"

#include "Engine.h"
#include "cRenderManager.h"

cSpriteModel::cSpriteModel()
{
	currSpriteId = 0;
	shaderName = "sprite";
}

void cSpriteModel::SetUpUniforms()
{
	Manager::render.SetupSpriteSheet(textureName, currSpriteId);
}
