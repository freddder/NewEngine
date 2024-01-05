#include "cSpriteModel.h"
#include "cRenderManager.h"
#include "cTextureManager.h"

cSpriteModel::cSpriteModel()
{
	currSpriteId = 0;
	shaderName = "sprite";
}

void cSpriteModel::SetUpUniforms()
{
	cTextureManager::GetInstance()->SetupSpriteSheet(textureName, currSpriteId);
}
