#include "cWaveModel.h"
#include "cRenderManager.h"


cWaveModel::cWaveModel()
{
	shaderName = "wave";
	textureOffset = glm::vec3(0);
	timer = 0.f;

	animation = new cSinAnimation(textureOffset, 2, 0);
	static_cast<cSinAnimation*>(animation)->AddKeyFrame(sKeyFrameVec3(7.f, glm::vec3(360.f, 0.f, 0.f)));
	animation->isRepeat = true;
}

cWaveModel::~cWaveModel()
{
	delete animation;
}

void cWaveModel::SetUpUniforms()
{
	timer += 0.0043f;

	cRenderManager* renderManager = cRenderManager::GetInstance();
	renderManager->setVec2("UVoffset", textureOffset);
	renderManager->setFloat("timer", timer);
}
