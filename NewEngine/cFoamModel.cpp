#include "cFoamModel.h"
#include "cRenderManager.h"
#include "cSinAnimation.h"

cFoamModel::cFoamModel()
{
	shaderName = "foam";
	textureOffset = glm::vec3(0);

	animation = new cSinAnimation(textureOffset, 2, 0);
	static_cast<cSinAnimation*>(animation)->AddKeyFrame(sKeyFrameVec3(7.f, glm::vec3(360.f, 0.f, 0.f)));
	animation->isRepeat = true;
}

cFoamModel::~cFoamModel()
{
}

void cFoamModel::SetUpUniforms()
{
	cRenderManager::GetInstance()->setVec2("UVoffset", textureOffset);
}
