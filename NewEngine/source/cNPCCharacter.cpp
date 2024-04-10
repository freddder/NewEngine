#include "cNPCCharacter.h"
#include "cMapManager.h"
#include "cAnimationManager.h"

cNPCCharacter::cNPCCharacter(glm::vec3 position, std::string textureName) : cCharacter(position, textureName)
{
	blocksTile = true;

	switchLeg = false;
}

cNPCCharacter::~cNPCCharacter()
{
}

void cNPCCharacter::Move(eDirection dir, bool run)
{
	if (!modelAnimation->isDone) return;

	eEntityMoveResult moveResult = ProcessMovement(dir, run);

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
	cAnimationManager::GetInstance()->GetSpriteAnimationKeyframes(NPC, animationName, keyframes);
	spriteAnimation->AddKeyFrames(keyframes);
	switchLeg = !switchLeg;

	if (run) spriteAnimation->speed = 2.f;
	else spriteAnimation->speed = 1.f;

	if (follower && moveResult != eEntityMoveResult::FAILURE)
	{
		MoveFollower(model->position, run);
	}
}
