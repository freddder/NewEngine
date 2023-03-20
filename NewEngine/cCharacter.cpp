#include "cCharacter.h"
#include "cSpriteModel.h"
#include "cRenderManager.h"
#include "cMapManager.h"
#include "cAnimationManager.h"
#include "cTextureManager.h"
//#include <iostream>

cCharacter::cCharacter(glm::vec3 position, std::string textureName)
{
	model = new cSpriteModel();
	model->meshName = "SpriteHolder.obj";
	model->position = position;
	model->textureName = textureName;
	//model->textureAnimationType = Sprite;

	cRenderManager::GetInstance()->AddModel(model);

	spriteAnimation = new cSpriteAnimation(model->currSpriteId, model->scale);
	modelAnimation = new cModelAnimation(model->position, model->orientation, model->scale);

	cAnimationManager::GetInstance()->AddAnimation(spriteAnimation);
	cAnimationManager::GetInstance()->AddAnimation(modelAnimation);

	switchLeg = false;
}

cCharacter::~cCharacter()
{
	cRenderManager::GetInstance()->RemoveModel(model);

	cAnimationManager::GetInstance()->RemoveAnimation(spriteAnimation);
	cAnimationManager::GetInstance()->RemoveAnimation(modelAnimation);
	//g_AnimationManager->RemoveAnimation(spriteAnimation);
	//g_AnimationManager->RemoveAnimation(modelAnimation);

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

	if (cTextureManager::GetInstance()->GetSpritesheetSymetry(model->textureName)) // is symetrical
	{
		if (dir == UP)
		{
			if (switchLeg) spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.01f, 1, false)); // true or false
			else spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.01f, 1, true));
			spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.2f, 0, false));
			spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.3f, 0, false));
		}
		else if (dir == DOWN)
		{
			if (switchLeg) spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.01f, 3, false)); // true or false
			else spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.01f, 3, true));
			spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.2f, 2, false));
			spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.3f, 2, false));
		}
		else if (dir == LEFT)
		{
			if (switchLeg) spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.01f, 5, false)); // 5 or 6
			else spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.01f, 6, false));
			spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.2f, 4, false));
			spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.3f, 4, false));
		}
		else if (dir == RIGHT)
		{
			if (switchLeg) spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.01f, 5, true)); // 5 or 6 and
			else spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.01f, 6, true));
			spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.2f, 4, true));
			spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.3f, 4, true));
		}
	}
	else // is asymetrical
	{
		if (dir == UP)
		{
			if (switchLeg) spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.01f, 1, false)); // 1 or 2
			else spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.01f, 2, false));
			spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.2f, 0, false));
			spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.3f, 0, false));
		}
		else if (dir == DOWN)
		{
			if (switchLeg) spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.01f, 4, false)); // 4 or 5
			else spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.01f, 5, false));
			spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.2f, 3, false));
			spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.3f, 3, false));
		}
		else if (dir == LEFT)
		{
			if (switchLeg) spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.01f, 7, false)); // 7 or 8
			else spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.01f, 8, false));
			spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.2f, 6, false));
			spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.3f, 6, false));
		}
		else if (dir == RIGHT)
		{
			if (switchLeg) spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.01f, 10, false)); // 10 or 11
			else spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.01f, 11, false));
			spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.2f, 9, false));
			spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.3f, 9, false));
		}
	}

	switchLeg = !switchLeg;

	// if model animation isnt done return (this is also where it should check if its possible to walk to next tile. maybe pass a bool)
	if (!modelAnimation->isDone)
		return;

	int moveResult = cMapManager::GetInstance()->TryMoveEntity(model->position, dir);

	if (moveResult == 0) return;

	// set up model animation
	modelAnimation->Reset(model->position, model->orientation, model->scale);

	glm::vec3 newPosition = model->position;

	if (moveResult == 2)
		newPosition.y += 1.f;
	else if (moveResult == 3)
		newPosition.y -= 1.f;

	if (dir == UP)
		newPosition.x += 1.f;
	else if (dir == DOWN)
		newPosition.x -= 1.f;
	else if (dir == LEFT)
		newPosition.z -= 1.f;
	else if (dir == RIGHT)
		newPosition.z += 1.f;

	modelAnimation->AddPositionKeyFrame(sKeyFrameVec3(0.3f, newPosition));
}
