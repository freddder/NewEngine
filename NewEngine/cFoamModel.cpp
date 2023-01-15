#include "cFoamModel.h"
#include "cRenderManager.h"

cFoamModel::cFoamModel()
{
	shaderName = "foam";
	textureOffset = glm::vec3(0);
}

void cFoamModel::SetUpUniforms()
{
	cRenderManager::GetInstance()->setVec2("UVoffset", textureOffset);
}
