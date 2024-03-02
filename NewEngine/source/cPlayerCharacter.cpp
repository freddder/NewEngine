#include "cPlayerCharacter.h"
#include "cSpriteModel.h"
#include "cRenderManager.h"
#include "cMapManager.h"
#include "cAnimationManager.h"

cPlayerCharacter::cPlayerCharacter(glm::vec3 position) : cCharacter(position, "Nate.png")
{
	spriteAnimation->isRepeat = true;
	lastDesiredDirection = DOWN;
}

cPlayerCharacter::~cPlayerCharacter()
{
}

void cPlayerCharacter::Move(eDirection dir, bool run)
{
	if (!modelAnimation->isDone) return;

	int moveResult = ProcessMovement(dir, run);

	if (!run || moveResult == 0) SetupSpriteWalk(dir);
	else SetupSpriteRun(dir);

	lastDesiredDirection = dir;

	if (follower && moveResult != 0) MoveFollower(model->position, run);
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

void cPlayerCharacter::SetupSpriteWalk(eDirection dir)
{
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

void cPlayerCharacter::SetupSpriteRun(eDirection dir)
{		
	if (lastDesiredDirection != dir || spriteAnimation->keyframes.size() != 5) // <-- very hacky
	{
		spriteAnimation->Reset(model->currSpriteId, model->scale);

		std::string animationName;
		if (dir == UP) animationName = "RUN_UP";
		else if (dir == DOWN) animationName = "RUN_DOWN";
		else if (dir == LEFT) animationName = "RUN_LEFT";
		else if (dir == RIGHT) animationName = "RUN_RIGHT";

		std::vector<sKeyFrameSprite> keyframes;
		cAnimationManager::GetInstance()->GetSpriteAnimationKeyframes(PLAYER, animationName, keyframes);
		spriteAnimation->AddKeyFrames(keyframes);
	}
}
