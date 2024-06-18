#include "CharacterSprite.h"

#include "Engine.h"
#include "cRenderManager.h"
#include "cAnimationManager.h"
#include "cMapManager.h"

cCharacterSprite::cCharacterSprite(std::string textureName, glm::vec3 pos)
{
	model = Manager::render.CreateSpriteModel();
	model->meshName = "SpriteHolder.obj";
	model->position = pos;
	model->textureName = textureName;

	spriteAnimation = std::make_shared<cSpriteAnimation>(model->currSpriteId, model->scale);
	modelAnimation = std::make_shared<cModelAnimation>(model->position, model->orientation, model->scale);

	Manager::animation.AddAnimation(spriteAnimation);
	Manager::animation.AddAnimation(modelAnimation);
}

cCharacterSprite::~cCharacterSprite()
{
	Manager::render.RemoveModel(model);

	Manager::animation.RemoveAnimation(spriteAnimation);
	Manager::animation.RemoveAnimation(modelAnimation);
}

glm::vec3 cCharacterSprite::AnimateMovement(eDirection dir, bool run, eEntityMoveResult moveResult)
{
	modelAnimation->Reset(model->position, model->orientation, model->scale);

	// Make model animation
	glm::vec3 newPosition = model->position;

	if (moveResult != eEntityMoveResult::FAILURE)
	{
		if (dir == UP) newPosition.x += 1.f;
		else if (dir == DOWN) newPosition.x -= 1.f;
		else if (dir == LEFT) newPosition.z -= 1.f;
		else if (dir == RIGHT) newPosition.z += 1.f;
	}

	// Ajust height
	if (moveResult == eEntityMoveResult::SUCCESS_UP) newPosition.y += 1.f;
	else if (moveResult == eEntityMoveResult::SUCCESS_DOWN) newPosition.y -= 1.f;

	if (!run || moveResult == 0) modelAnimation->AddPositionKeyFrame(sKeyFrameVec3(0.3f, newPosition));
	else modelAnimation->AddPositionKeyFrame(sKeyFrameVec3(0.14f, newPosition));

	return newPosition;
}

cOverworldPokemonSprite::cOverworldPokemonSprite(std::string textureName, glm::vec3 pos) : cCharacterSprite(textureName, pos)
{
	lastDesiredDirection = DOWN;
	spriteAnimation->isRepeat = true;

	std::vector<sKeyFrameSprite> keyframes;
	Manager::animation.GetSpriteAnimationKeyframes(OW_POKEMON, "WALK_DOWN", keyframes);
	spriteAnimation->AddKeyFrames(keyframes);
}

cOverworldPokemonSprite::~cOverworldPokemonSprite()
{
	
}

glm::vec3 cOverworldPokemonSprite::AnimateMovement(eDirection dir, bool run, eEntityMoveResult moveResult)
{
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
		Manager::animation.GetSpriteAnimationKeyframes(OW_POKEMON, animationName, keyframes);
		spriteAnimation->AddKeyFrames(keyframes);
	}

	lastDesiredDirection = dir;

	return cCharacterSprite::AnimateMovement(dir, run, moveResult);
}

cNPCSprite::cNPCSprite(std::string textureName, glm::vec3 pos) : cCharacterSprite(textureName, pos)
{
	switchLeg = false;
}

cNPCSprite::~cNPCSprite()
{

}

glm::vec3 cNPCSprite::AnimateMovement(eDirection dir, bool run, eEntityMoveResult moveResult)
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
	Manager::animation.GetSpriteAnimationKeyframes(NPC, animationName, keyframes);
	spriteAnimation->AddKeyFrames(keyframes);
	switchLeg = !switchLeg;

	if (run) spriteAnimation->speed = 2.f;
	else spriteAnimation->speed = 1.f;

	return cCharacterSprite::AnimateMovement(dir, run, moveResult);
}

cPlayerSprite::cPlayerSprite(std::string textureName, glm::vec3 pos) : cCharacterSprite(textureName, pos)
{
	lastDesiredDirection = DOWN;
	switchLeg = false;

	spriteAnimation->isRepeat = true;
}

cPlayerSprite::~cPlayerSprite()
{
	
}

void cPlayerSprite::SetupSpriteWalk(eDirection dir)
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
	Manager::animation.GetSpriteAnimationKeyframes(PLAYER, animationName, keyframes);
	spriteAnimation->AddKeyFrames(keyframes);
	switchLeg = !switchLeg;
}

void cPlayerSprite::SetupSpriteRun(eDirection dir)
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
		Manager::animation.GetSpriteAnimationKeyframes(PLAYER, animationName, keyframes);
		spriteAnimation->AddKeyFrames(keyframes);
	}
}
glm::vec3 cPlayerSprite::AnimateMovement(eDirection dir, bool run, eEntityMoveResult moveResult)
{
	if (!run || moveResult == 0) SetupSpriteWalk(dir);
	else SetupSpriteRun(dir);

	lastDesiredDirection = dir;

	return cCharacterSprite::AnimateMovement(dir, run, moveResult);
}

void cPlayerSprite::StopMovement()
{
	if (!modelAnimation->isDone) return;

	spriteAnimation->Reset();
	modelAnimation->Reset();

	if (lastDesiredDirection == UP) model->currSpriteId = 0;
	else if (lastDesiredDirection == DOWN) model->currSpriteId = 3;
	else if (lastDesiredDirection == LEFT) model->currSpriteId = 6;
	else if (lastDesiredDirection == RIGHT) model->currSpriteId = 9;
}

cBattleSprite::cBattleSprite(std::string textureName, glm::vec3 pos, float spriteHeightSize, float spriteAspectRatio, int spritesNum)
{
	model = Manager::render.CreateSpriteModel();
	model->meshName = "SpriteHolder.obj";
	model->position = pos;
	model->scale.x = spriteHeightSize * spriteAspectRatio;
	model->scale.y = spriteHeightSize;
	model->textureName = textureName;

	spriteAnimation = std::make_shared<cPeriodicSpriteAnimation>(model->currSpriteId, 82);
	spriteAnimation->isRepeat = true;

	Manager::animation.AddAnimation(spriteAnimation);
}

cBattleSprite::~cBattleSprite()
{
	Manager::render.RemoveModel(model);

	Manager::animation.RemoveAnimation(spriteAnimation);
}
