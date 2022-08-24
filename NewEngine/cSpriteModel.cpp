#include "cSpriteModel.h"
#include "Global.h"

cSpriteModel::cSpriteModel()
{
	currSpriteId = 0;
	shaderName = "sprite";
}

void cSpriteModel::SetUpUniforms()
{
	g_RenderManager->setInt("spriteId", currSpriteId);

	sSpriteSheet sheet;
	g_TextureManager->GetSpriteSheet(textureName, sheet);

	g_RenderManager->setInt("numCols", sheet.numCols);
	g_RenderManager->setInt("numRows", sheet.numRows);
}
