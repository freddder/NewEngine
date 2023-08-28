#pragma once
#include "cRenderModel.h"
#include "cAnimation.h"

class cAnimatedModel : public cRenderModel
{
public:
	cAnimation* animation;

	cAnimatedModel();
	~cAnimatedModel();
};