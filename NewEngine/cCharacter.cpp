#include "cCharacter.h"
#include "Global.h"

cCharacter::cCharacter()
{
	model = new cModel();
	model->meshName = "SpriteHolder.obj";

	spriteAnimation = new cSpriteAnimation(model->currSpriteId, model->scale);
	modelAnimation = new cModelAnimation(model->position, model->orientation, model->scale);

	spriteAnimation->isDone = false;
	modelAnimation->isDone = false;

	g_AnimationManager->AddAnimation(spriteAnimation);
	g_AnimationManager->AddAnimation(modelAnimation);
}

cCharacter::~cCharacter()
{
	g_AnimationManager->RemoveAnimation(spriteAnimation);
	g_AnimationManager->RemoveAnimation(modelAnimation);

	delete spriteAnimation;
	delete modelAnimation;

	delete model;
}

void cCharacter::Walk(eDirection dir)
{
	// if sprite animation isnt done no point on proceeding
	if (!spriteAnimation->isDone)
		return;

	// setup sprite animation into specific direction
	spriteAnimation->Reset(model->currSpriteId, model->scale);



	// if model animation isnt done return (this is also where it should check if its possible to walk to next tile. maybe pass a bool)
	if (!modelAnimation->isDone)
		return;

	// set up model animation
}
