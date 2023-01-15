#include "cWaveModel.h"
#include "cRenderManager.h"


cWaveModel::cWaveModel()
{
	shaderName = "wave";
	textureOffset = glm::vec3(0);
	timer = 0.f;
}

void cWaveModel::SetUpUniforms()
{
	timer += 0.0043f;

	cRenderManager* renderManager = cRenderManager::GetInstance();
	renderManager->setVec2("UVoffset", textureOffset);
	renderManager->setFloat("timer", timer);
}
