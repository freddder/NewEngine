#include "cOceanModel.h"
#include "Global.h"

cOceanModel::cOceanModel()
{
	shaderName = "ocean";
	globalUVRatios = glm::vec2(1.f);
	textureOffset = glm::vec3(0.f);
}

void cOceanModel::SetUpUniforms()
{
	g_RenderManager->setVec2("globalUVRatios", globalUVRatios);
	g_RenderManager->setVec2("UVoffset", textureOffset);
}
