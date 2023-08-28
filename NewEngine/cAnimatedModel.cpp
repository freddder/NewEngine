#include "cAnimatedModel.h"

cAnimatedModel::cAnimatedModel()
{
}

cAnimatedModel::~cAnimatedModel()
{
	if (animation)
		delete animation;
}
