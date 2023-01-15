#include "cSpriteModel.h"
#include "Global.h"
#include "cRenderManager.h"

cSpriteModel::cSpriteModel()
{
	currSpriteId = 0;
	shaderName = "sprite";
}

void cSpriteModel::SetUpUniforms()
{
	cRenderManager* renderManager = cRenderManager::GetInstance();

	sSpriteSheet sheet;
	g_TextureManager->GetSpriteSheet(textureName, sheet);

	renderManager->setInt("spriteId", currSpriteId);
	renderManager->setInt("numCols", sheet.numCols);
	renderManager->setInt("numRows", sheet.numRows);
}
