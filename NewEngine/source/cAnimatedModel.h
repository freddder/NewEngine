#pragma once
#include "cRenderModel.h"
#include "cAnimation.h"
#include <memory>

class cAnimatedModel : public cRenderModel
{
public:
	std::shared_ptr<cAnimation> animation;

	cAnimatedModel();
	~cAnimatedModel();
};

class cFoamModel : public cAnimatedModel
{
public:

	glm::vec3 textureOffset;

	cFoamModel();
	~cFoamModel();

	virtual void SetUpUniforms();
};

class cOceanModel : public cAnimatedModel
{
public:

	glm::vec2 globalUVRatios;
	glm::vec3 textureOffset;
	float timer;

	cOceanModel();
	~cOceanModel();

	virtual void SetUpUniforms();
};

class cWaveModel : public cAnimatedModel
{
public:

	glm::vec3 textureOffset;
	float timer;

	cWaveModel();
	~cWaveModel();

	virtual void SetUpUniforms();
};

class cTreeModel : public cAnimatedModel
{
public:

	float timer;
	glm::vec3 dummy;

	cTreeModel();
	~cTreeModel();

	virtual void SetUpUniforms();
};