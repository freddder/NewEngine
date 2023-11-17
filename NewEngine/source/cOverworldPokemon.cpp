#include "cOverworldPokemon.h"
#include "cMapManager.h"
#include "cAnimationManager.h"

cOverworldPokemon::cOverworldPokemon(glm::vec3 position, std::string textureName) : cCharacter(position, textureName)
{
	lastDesiredDirection = DOWN;
	spriteAnimation->isRepeat = true;

	std::vector<sKeyFrameSprite> keyframes;
	cAnimationManager::GetInstance()->GetSpriteAnimationKeyframes(OW_POKEMON, "WALK_DOWN", keyframes);
	spriteAnimation->AddKeyFrames(keyframes);
}

cOverworldPokemon::~cOverworldPokemon()
{
}

void cOverworldPokemon::Move(eDirection dir, bool run)
{
	if (!modelAnimation->isDone) return;

	int moveResult = ProcessMovement(dir, run);

	// Change sprite animation
	if (lastDesiredDirection != dir)
	{
		spriteAnimation->Reset();
		std::string animationName;
		if (dir == UP) animationName = "WALK_UP";
		else if (dir == DOWN) animationName = "WALK_DOWN";
		else if (dir == LEFT) animationName = "WALK_LEFT";
		else if (dir == RIGHT) animationName = "WALK_RIGHT";

		std::vector<sKeyFrameSprite> keyframes;
		cAnimationManager::GetInstance()->GetSpriteAnimationKeyframes(OW_POKEMON, animationName, keyframes);
		spriteAnimation->AddKeyFrames(keyframes);
	}

	lastDesiredDirection = dir;

	if (follower && moveResult != 0) MoveFollower(model->position, run);
}