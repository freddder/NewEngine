#include "cAnimatedModel.h"

#include "Engine.h"
#include "cRenderManager.h"
#include "cSceneManager.h"

cAnimatedModel::cAnimatedModel()
{
}

cAnimatedModel::~cAnimatedModel()
{
}

cFoamModel::cFoamModel()
{
	shaderName = "foam";
	textureOffset = glm::vec3(0);

	animation = std::make_shared<cSinAnimation>(textureOffset, 2, 0);
	std::static_pointer_cast<cSinAnimation>(animation)->AddKeyFrame(sKeyFrameVec3(7.f, glm::vec3(360.f, 0.f, 0.f)));
	animation->isRepeat = true;
}

cFoamModel::~cFoamModel()
{
}

void cFoamModel::SetUpUniforms()
{
	Manager::render.setVec2("UVoffset", textureOffset);
}

cOceanModel::cOceanModel()
{
	shaderName = "ocean";
	globalUVRatios = glm::vec2(0.35f);
	textureOffset = glm::vec3(0.f);
	timer = 0.f;

	animation = std::make_shared<cSinAnimation>(textureOffset, 0.5f, 0);
	std::shared_ptr<cSinAnimation> waterOscilate = std::static_pointer_cast<cSinAnimation>(animation);
	waterOscilate->AddKeyFrame(sKeyFrameVec3(6.f, glm::vec3(360.f, 180.f, 0.f)));
	waterOscilate->AddKeyFrame(sKeyFrameVec3(12.f, glm::vec3(720.f, 360.f, 0.f)));
	waterOscilate->isRepeat = true;
}

cOceanModel::~cOceanModel()
{
}

void cOceanModel::SetUpUniforms()
{
	// TODO: use deltaTime for the love of god (and in other animated models)
	timer += 0.0043f;

	Manager::render.setVec2("globalUVRatios", globalUVRatios);
	Manager::render.setVec2("UVoffset", textureOffset);
	Manager::render.setFloat("timer", timer);
}

cWaveModel::cWaveModel()
{
	shaderName = "wave";
	textureOffset = glm::vec3(0);
	timer = 0.f;

	animation = std::make_shared<cSinAnimation>(textureOffset, 2, 0);
	std::static_pointer_cast<cSinAnimation>(animation)->AddKeyFrame(sKeyFrameVec3(7.f, glm::vec3(360.f, 0.f, 0.f)));
	animation->isRepeat = true;
}

cWaveModel::~cWaveModel()
{
}

void cWaveModel::SetUpUniforms()
{
	timer += 0.0043f;

	Manager::render.setVec2("UVoffset", textureOffset);
	Manager::render.setFloat("timer", timer);
}

cTreeModel::cTreeModel()
{
	shaderName = "tree";
	timer = 0.f;
}

cTreeModel::~cTreeModel()
{
}

void cTreeModel::SetUpUniforms()
{
	timer += 0.0043f;

	Manager::render.setFloat("timer", timer);
	Manager::render.setFloat("windSpeed", Manager::scene.windSpeed);
}
