#include "cOceanModel.h"
#include "cRenderManager.h"

cOceanModel::cOceanModel()
{
	shaderName = "ocean";
	globalUVRatios = glm::vec2(0.35f);
	textureOffset = glm::vec3(0.f);
	timer = 0.f;

	animation = new cSinAnimation(textureOffset, 0.5f, 0); // <-- might need to move this away from the contructor. will see...
	cSinAnimation* waterOscilate = static_cast<cSinAnimation*>(animation);
	waterOscilate->AddKeyFrame(sKeyFrameVec3(6.f, glm::vec3(360.f, 180.f, 0.f)));
	waterOscilate->AddKeyFrame(sKeyFrameVec3(12.f, glm::vec3(720.f, 360.f, 0.f)));
	waterOscilate->isRepeat = true;
}

cOceanModel::~cOceanModel()
{
}

void cOceanModel::SetUpUniforms()
{
	timer += 0.0043f;

	cRenderManager* renderManager = cRenderManager::GetInstance();
	renderManager->setVec2("globalUVRatios", globalUVRatios);
	renderManager->setVec2("UVoffset", textureOffset);
	renderManager->setFloat("timer", timer);
}
