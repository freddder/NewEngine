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

	delete spriteAnimation;
	delete modelAnimation;

	delete model;
}

void cCharacter::Walk(eDirection dir)
{
	if (!modelAnimation->isDone) return;
	
	modelAnimation->Reset(model->position, model->orientation, model->scale);
	
	// Make model animation
	int moveResult = cMapManager::GetInstance()->TryMoveEntity(model->position, dir);
	glm::vec3 newPosition = model->position;
	
	if (moveResult != 0)
	{
		if (dir == UP) newPosition.x += 1.f;
		else if (dir == DOWN) newPosition.x -= 1.f;
		else if (dir == LEFT) newPosition.z -= 1.f;
		else if (dir == RIGHT) newPosition.z += 1.f;
	}
	
	// Ajust height
	if (moveResult == 2) newPosition.y += 1.f;
	else if (moveResult == 3) newPosition.y -= 1.f;
	
	modelAnimation->AddPositionKeyFrame(sKeyFrameVec3(0.3f, newPosition));
	spriteAnimation->Reset();

	std::string animationName;
	if (dir == UP)
	{
		if (switchLeg) animationName = "WALK_UP_L";
		else animationName = "WALK_UP_R";
	}
	else if (dir == DOWN)
	{
		if (switchLeg) animationName = "WALK_DOWN_L";
		else animationName = "WALK_DOWN_R";
	}
	else if (dir == LEFT)
	{
		if (switchLeg) animationName = "WALK_LEFT_L";
		else animationName = "WALK_LEFT_R";
	}
	else if (dir == RIGHT)
	{
		if (switchLeg) animationName = "WALK_RIGHT_L";
		else animationName = "WALK_RIGHT_R";
	}

	std::vector<sKeyFrameSprite> keyframes;
	cAnimationManager::GetInstance()->GetSpriteAnimationKeyframes(PLAYER, animationName, keyframes);
	spriteAnimation->AddKeyFrames(keyframes);
	switchLeg = !switchLeg;
}
