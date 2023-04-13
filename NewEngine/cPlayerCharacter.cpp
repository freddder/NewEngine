#include "cPlayerCharacter.h"
#include "cSpriteModel.h"
#include "cRenderManager.h"
#include "cMapManager.h"
#include "cAnimationManager.h"
#include "cTextureManager.h"

cPlayerCharacter::cPlayerCharacter(glm::vec3 position) : cCharacter(position, "Nate.png")
{
	putLegForward = true;
}

cPlayerCharacter::~cPlayerCharacter()
{
}

void cPlayerCharacter::Run(eDirection dir)
{
	// if sprite animation isnt done no point on proceeding
	if (!spriteAnimation->isDone)
		return;

	// setup sprite animation into specific direction
	spriteAnimation->Reset(model->currSpriteId, model->scale);

	int moveResult = cMapManager::GetInstance()->TryMoveEntity(model->position, dir);

	if (moveResult == 0)
	{
		// walk instead
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

		switchLeg = !switchLeg;

		return;
	}

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

	modelAnimation->AddPositionKeyFrame(sKeyFrameVec3(0.14f, newPosition));

	if (dir == UP)
	{
		if (putLegForward)
		{
			if (switchLeg)
			{
				spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.01f, 13, false));
				spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.14f, 13, false));
			}
			else
			{
				spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.01f, 14, false));
				spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.14f, 14, false));
			}
			switchLeg = !switchLeg;
		}
		else
		{
			spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.01f, 12, false));
			spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.14f, 12, false));
		}
		spriteAnimation->callback = [this]() { model->currSpriteId = 0; };
	}
	else if (dir == DOWN)
	{
		if (putLegForward)
		{
			if (switchLeg)
			{
				spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.01f, 16, false));
				spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.14f, 16, false));
			}
			else
			{
				spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.01f, 17, false));
				spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.14f, 17, false));
			}
			switchLeg = !switchLeg;
		}
		else
		{
			spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.01f, 15, false));
			spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.14f, 15, false));
		}
		spriteAnimation->callback = [this]() { model->currSpriteId = 3; };
	}
	else if (dir == LEFT)
	{
		if (putLegForward)
		{
			if (switchLeg)
			{
				spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.01f, 19, false));
				spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.14f, 19, false));
			}
			else
			{
				spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.01f, 20, false));
				spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.14f, 20, false));
			}
			switchLeg = !switchLeg;
		}
		else
		{
			spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.01f, 18, false));
			spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.14f, 18, false));
		}
		spriteAnimation->callback = [this]() { model->currSpriteId = 6; };
	}
	else if (dir == RIGHT)
	{
		if (putLegForward)
		{
			if (switchLeg)
			{
				spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.01f, 22, false));
				spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.14f, 22, false));
			}
			else
			{
				spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.01f, 23, false));
				spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.14f, 23, false));
			}
			switchLeg = !switchLeg;
		}
		else
		{
			spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.01f, 21, false));
			spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.14f, 21, false));
		}
		spriteAnimation->callback = [this]() { model->currSpriteId = 9; };
	}

	putLegForward = !putLegForward;
}
