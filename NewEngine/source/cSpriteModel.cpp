#include "cSpriteModel.h"
#include "cRenderManager.h"
#include "cRenderManager.h"

cSpriteModel::cSpriteModel()
{
	currSpriteId = 0;
	shaderName = "sprite";
}

void cSpriteModel::SetUpUniforms()
{
	cRenderManager::GetInstance()->SetupSpriteSheet(textureName, currSpriteId);
}
