#include "cOceanModel.h"
#include "cRenderManager.h"

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

	cRenderManager* renderManager = cRenderManager::GetInstance();
	renderManager->setVec2("globalUVRatios", globalUVRatios);
	renderManager->setVec2("UVoffset", textureOffset);
	renderManager->setFloat("timer", timer);
}
