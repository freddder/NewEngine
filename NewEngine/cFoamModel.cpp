#include "cFoamModel.h"
#include "Global.h"

cFoamModel::cFoamModel()
{
	shaderName = "foam";
	textureOffset = glm::vec3(0);
}

void cFoamModel::SetUpUniforms()
{
	g_RenderManager->setVec2("UVoffset", textureOffset);
}
