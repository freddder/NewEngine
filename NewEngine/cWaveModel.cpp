#include "cWaveModel.h"
#include "Global.h"

cWaveModel::cWaveModel()
{
	shaderName = "wave";
	textureOffset = glm::vec3(0);
}

void cWaveModel::SetUpUniforms()
{
	g_RenderManager->setVec2("UVoffset", textureOffset);
}
