#include "cOceanModel.h"
#include "Global.h"

cOceanModel::cOceanModel()
{
	shaderName = "ocean";
	globalUVRatios = glm::vec2(1.f);
	textureOffset = glm::vec3(0.f);
	timer = 0.f;
}

void cOceanModel::SetUpUniforms()
{
	timer += 0.0043f;

	g_RenderManager->setVec2("globalUVRatios", globalUVRatios);
	g_RenderManager->setVec2("UVoffset", textureOffset);
	g_RenderManager->setFloat("timer", timer);
}
