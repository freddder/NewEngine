#include "cWaveModel.h"
#include "Global.h"

cWaveModel::cWaveModel()
{
	shaderName = "wave";
	textureOffset = glm::vec3(0);
	timer = 0.f;
}

void cWaveModel::SetUpUniforms()
{
	timer += 0.0043f;

	g_RenderManager->setVec2("UVoffset", textureOffset);
	g_RenderManager->setFloat("timer", timer);
}
