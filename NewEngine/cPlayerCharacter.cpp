#include "cPlayerCharacter.h"
#include "cSpriteModel.h"
#include "cRenderManager.h"
#include "cMapManager.h"
#include "cAnimationManager.h"
#include "cTextureManager.h"

cPlayerCharacter::cPlayerCharacter(glm::vec3 position) : cCharacter(position, "Nate.png")
{
	spriteAnimation->isRepeat = true;
	lastDesiredDirection = DOWN;
}

cPlayerCharacter::~cPlayerCharacter()
{
}

void cPlayerCharacter::Walk(eDirection dir)
{
	cCharacter::Walk(dir);

	lastDesiredDirection = dir;
}

void cPlayerCharacter::Run(eDirection dir)
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
	
	if (moveResult == 0) // walk instead
	{
		modelAnimation->AddPositionKeyFrame(sKeyFrameVec3(0.3f, newPosition));
		spriteAnimation->Reset(model->currSpriteId, model->scale);
		
		if (dir == UP)
		{
			if (switchLeg) spriteAnimation->AddKeyFrames(KEYFRAMES_WALK_UP_L);
			else spriteAnimation->AddKeyFrames(KEYFRAMES_WALK_UP_R);
		}
		else if (dir == DOWN)
		{
			if (switchLeg) spriteAnimation->AddKeyFrames(KEYFRAMES_WALK_DOWN_L);
			else spriteAnimation->AddKeyFrames(KEYFRAMES_WALK_DOWN_R);
		}
		else if (dir == LEFT)
		{
			if (switchLeg) spriteAnimation->AddKeyFrames(KEYFRAMES_WALK_LEFT_L);
			else spriteAnimation->AddKeyFrames(KEYFRAMES_WALK_LEFT_R);
		}
		else if (dir == RIGHT)
		{
			if (switchLeg) spriteAnimation->AddKeyFrames(KEYFRAMES_WALK_RIGHT_L);
			else spriteAnimation->AddKeyFrames(KEYFRAMES_WALK_RIGHT_R);
		}

		switchLeg = !switchLeg;
	}
	else
	{
		modelAnimation->AddPositionKeyFrame(sKeyFrameVec3(0.14f, newPosition));

		if(lastDesiredDirection != dir) spriteAnimation->Reset(model->currSpriteId, model->scale);

		if (dir == UP) spriteAnimation->AddKeyFrames(KEYFRAMES_RUN_UP);
		else if (dir == DOWN) spriteAnimation->AddKeyFrames(KEYFRAMES_RUN_DOWN);
		else if (dir == LEFT) spriteAnimation->AddKeyFrames(KEYFRAMES_RUN_LEFT);
		else if (dir == RIGHT) spriteAnimation->AddKeyFrames(KEYFRAMES_RUN_RIGHT);

		lastDesiredDirection = dir;
	}
}

void cPlayerCharacter::StopMovement()
{
	if (!modelAnimation->isDone) return;

	spriteAnimation->Reset();
	modelAnimation->Reset();

	if (lastDesiredDirection == UP) model->currSpriteId = 0;
	else if (lastDesiredDirection == DOWN) model->currSpriteId = 3;
	else if (lastDesiredDirection == LEFT) model->currSpriteId = 6;
	else if (lastDesiredDirection == RIGHT) model->currSpriteId = 9;
}
