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
	cRenderManager* renderManager = cRenderManager::GetInstance();

	sSpriteSheet sheet;
	cTextureManager::GetInstance()->GetSpriteSheet(textureName, sheet);

	renderManager->setInt("spriteId", currSpriteId);
	renderManager->setInt("numCols", sheet.numCols);
	renderManager->setInt("numRows", sheet.numRows);
}
