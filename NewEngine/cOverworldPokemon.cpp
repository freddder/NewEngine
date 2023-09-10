#include "cOverworldPokemon.h"
#include "cMapManager.h"
#include "cAnimationManager.h"

cOverworldPokemon::cOverworldPokemon(glm::vec3 position, std::string textureName) : cCharacter(position, textureName)
{
	characterType = OW_POKEMON;
	lastDesiredDirection = DOWN;
	spriteAnimation->isRepeat = true;

	std::vector<sKeyFrameSprite> keyframes;
	cAnimationManager::GetInstance()->GetSpriteAnimationKeyframes(characterType, "WALK_DOWN", keyframes);
	spriteAnimation->AddKeyFrames(keyframes);
}

cOverworldPokemon::~cOverworldPokemon()
{
}

void cOverworldPokemon::Move(eDirection dir, bool run)
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

	// Move model
	if (!run || moveResult == 0)
	{
		modelAnimation->AddPositionKeyFrame(sKeyFrameVec3(0.3f, newPosition));		
	}
	else
	{
		modelAnimation->AddPositionKeyFrame(sKeyFrameVec3(0.14f, newPosition));		
	}

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
		cAnimationManager::GetInstance()->GetSpriteAnimationKeyframes(characterType, animationName, keyframes);
		spriteAnimation->AddKeyFrames(keyframes);
	}

	lastDesiredDirection = dir;

	if (follower && moveResult != 0) follower->Follow(model->position, run);
}

void cOverworldPokemon::Follow(glm::vec3 newPosition, bool run)
{
	eDirection dir = UP;
	if ((newPosition - model->position).z == 1.f) dir = RIGHT;
	else if ((newPosition - model->position).z == -1.f) dir = LEFT;
	else if ((newPosition - model->position).x == 1.f) dir = UP;
	else if ((newPosition - model->position).x == -1.f) dir = DOWN;
	else return;

	modelAnimation->isDone = true;

	Move(dir, run);
}
