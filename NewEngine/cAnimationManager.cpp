#include "cAnimationManager.h"
#include <vector>

cAnimationManager* cAnimationManager::singleton = NULL;

cAnimationManager::cAnimationManager()
{

}

void cAnimationManager::Process(float deltaTime)
{
	for (int i = animations.size() - 1; i >= 0; i--)
	{
		animations[i]->Process(deltaTime);

		if (animations[i]->timer >= animations[i]->maxDuration)
		{
			if (animations[i]->isRepeat)
				animations[i]->timer = 0.f;
			else
			{
				animations[i]->isDone = true;
				
				if(animations[i]->callback)
					animations[i]->callback();
			}
		}

		if (animations[i]->clearAfterComplete && animations[i]->isDone)
		{
			cAnimation* animationToDelete = animations[i];
			animations.erase(animations.begin() + i);
			delete animationToDelete;
		}
	}
}

void cAnimationManager::InitializeAnimationsPresets()
{
	std::vector<sKeyFrameSprite> WALK_UP_L;
	WALK_UP_L.push_back(sKeyFrameSprite(0.01f, 1));
	WALK_UP_L.push_back(sKeyFrameSprite(0.2f, 0));
	WALK_UP_L.push_back(sKeyFrameSprite(0.3f, 0));
	CreateSpriteAnimation(PLAYER, "WALK_UP_L", WALK_UP_L);
	CreateSpriteAnimation(NPC, "WALK_UP_L", WALK_UP_L);

	std::vector<sKeyFrameSprite> WALK_UP_R;
	WALK_UP_R.push_back(sKeyFrameSprite(0.01f, 2));
	WALK_UP_R.push_back(sKeyFrameSprite(0.2f, 0));
	WALK_UP_R.push_back(sKeyFrameSprite(0.3f, 0));
	CreateSpriteAnimation(PLAYER, "WALK_UP_R", WALK_UP_R);
	CreateSpriteAnimation(NPC, "WALK_UP_R", WALK_UP_R);

	std::vector<sKeyFrameSprite> WALK_DOWN_L;
	WALK_DOWN_L.push_back(sKeyFrameSprite(0.01f, 4));
	WALK_DOWN_L.push_back(sKeyFrameSprite(0.2f, 3));
	WALK_DOWN_L.push_back(sKeyFrameSprite(0.3f, 3));
	CreateSpriteAnimation(PLAYER, "WALK_DOWN_L", WALK_DOWN_L);
	CreateSpriteAnimation(NPC, "WALK_DOWN_L", WALK_DOWN_L);

	std::vector<sKeyFrameSprite> WALK_DOWN_R;
	WALK_DOWN_R.push_back(sKeyFrameSprite(0.01f, 5));
	WALK_DOWN_R.push_back(sKeyFrameSprite(0.2f, 3));
	WALK_DOWN_R.push_back(sKeyFrameSprite(0.3f, 3));
	CreateSpriteAnimation(PLAYER, "WALK_DOWN_R", WALK_DOWN_R);
	CreateSpriteAnimation(NPC, "WALK_DOWN_R", WALK_DOWN_R);

	std::vector<sKeyFrameSprite> WALK_LEFT_L;
	WALK_LEFT_L.push_back(sKeyFrameSprite(0.01f, 7));
	WALK_LEFT_L.push_back(sKeyFrameSprite(0.2f, 6));
	WALK_LEFT_L.push_back(sKeyFrameSprite(0.3f, 6));
	CreateSpriteAnimation(PLAYER, "WALK_LEFT_L", WALK_LEFT_L);
	CreateSpriteAnimation(NPC, "WALK_LEFT_L", WALK_LEFT_L);

	std::vector<sKeyFrameSprite> WALK_LEFT_R;
	WALK_LEFT_R.push_back(sKeyFrameSprite(0.01f, 8));
	WALK_LEFT_R.push_back(sKeyFrameSprite(0.2f, 6));
	WALK_LEFT_R.push_back(sKeyFrameSprite(0.3f, 6));
	CreateSpriteAnimation(PLAYER, "WALK_LEFT_R", WALK_LEFT_R);
	CreateSpriteAnimation(NPC, "WALK_LEFT_R", WALK_LEFT_R);

	std::vector<sKeyFrameSprite> WALK_RIGHT_L;
	WALK_RIGHT_L.push_back(sKeyFrameSprite(0.01f, 10));
	WALK_RIGHT_L.push_back(sKeyFrameSprite(0.2f, 9));
	WALK_RIGHT_L.push_back(sKeyFrameSprite(0.3f, 9));
	CreateSpriteAnimation(PLAYER, "WALK_RIGHT_L", WALK_RIGHT_L);
	CreateSpriteAnimation(NPC, "WALK_RIGHT_L", WALK_RIGHT_L);

	std::vector<sKeyFrameSprite> WALK_RIGHT_R;
	WALK_RIGHT_R.push_back(sKeyFrameSprite(0.01f, 11));
	WALK_RIGHT_R.push_back(sKeyFrameSprite(0.2f, 9));
	WALK_RIGHT_R.push_back(sKeyFrameSprite(0.3f, 9));
	CreateSpriteAnimation(PLAYER, "WALK_RIGHT_R", WALK_RIGHT_R);
	CreateSpriteAnimation(NPC, "WALK_RIGHT_R", WALK_RIGHT_R);

	std::vector<sKeyFrameSprite> RUN_UP;
	RUN_UP.push_back(sKeyFrameSprite(0.01f, 13));
	RUN_UP.push_back(sKeyFrameSprite(0.14f, 12));
	RUN_UP.push_back(sKeyFrameSprite(0.28f, 14));
	RUN_UP.push_back(sKeyFrameSprite(0.42f, 12));
	RUN_UP.push_back(sKeyFrameSprite(0.56f, 12));
	CreateSpriteAnimation(PLAYER, "RUN_UP", RUN_UP);

	std::vector<sKeyFrameSprite> RUN_DOWN;
	RUN_DOWN.push_back(sKeyFrameSprite(0.01f, 16));
	RUN_DOWN.push_back(sKeyFrameSprite(0.14f, 15));
	RUN_DOWN.push_back(sKeyFrameSprite(0.28f, 17));
	RUN_DOWN.push_back(sKeyFrameSprite(0.42f, 15));
	RUN_DOWN.push_back(sKeyFrameSprite(0.56f, 15));
	CreateSpriteAnimation(PLAYER, "RUN_DOWN", RUN_DOWN);

	std::vector<sKeyFrameSprite> RUN_LEFT;
	RUN_LEFT.push_back(sKeyFrameSprite(0.01f, 19));
	RUN_LEFT.push_back(sKeyFrameSprite(0.14f, 18));
	RUN_LEFT.push_back(sKeyFrameSprite(0.28f, 20));
	RUN_LEFT.push_back(sKeyFrameSprite(0.42f, 18));
	RUN_LEFT.push_back(sKeyFrameSprite(0.56f, 18));
	CreateSpriteAnimation(PLAYER, "RUN_LEFT", RUN_LEFT);

	std::vector<sKeyFrameSprite> RUN_RIGHT;
	RUN_RIGHT.push_back(sKeyFrameSprite(0.01f, 22));
	RUN_RIGHT.push_back(sKeyFrameSprite(0.14f, 21));
	RUN_RIGHT.push_back(sKeyFrameSprite(0.28f, 23));
	RUN_RIGHT.push_back(sKeyFrameSprite(0.42f, 21));
	RUN_RIGHT.push_back(sKeyFrameSprite(0.56f, 21));
	CreateSpriteAnimation(PLAYER, "RUN_RIGHT", RUN_RIGHT);

	std::vector<sKeyFrameSprite> SYM_WALK_UP_L;
	SYM_WALK_UP_L.push_back(sKeyFrameSprite(0.01f, 1, true));
	SYM_WALK_UP_L.push_back(sKeyFrameSprite(0.2f, 0));
	SYM_WALK_UP_L.push_back(sKeyFrameSprite(0.3f, 0));
	CreateSpriteAnimation(NPC, "SYM_WALK_UP_L", SYM_WALK_UP_L);

	std::vector<sKeyFrameSprite> SYM_WALK_UP_R;
	SYM_WALK_UP_R.push_back(sKeyFrameSprite(0.01f, 1));
	SYM_WALK_UP_R.push_back(sKeyFrameSprite(0.2f, 0));
	SYM_WALK_UP_R.push_back(sKeyFrameSprite(0.3f, 0));
	CreateSpriteAnimation(NPC, "SYM_WALK_UP_R", SYM_WALK_UP_R);

	std::vector<sKeyFrameSprite> SYM_WALK_DOWN_L;
	SYM_WALK_DOWN_L.push_back(sKeyFrameSprite(0.01f, 3, true));
	SYM_WALK_DOWN_L.push_back(sKeyFrameSprite(0.2f, 2));
	SYM_WALK_DOWN_L.push_back(sKeyFrameSprite(0.3f, 2));
	CreateSpriteAnimation(NPC, "SYM_WALK_DOWN_L", SYM_WALK_DOWN_L);

	std::vector<sKeyFrameSprite> SYM_WALK_DOWN_R;
	SYM_WALK_DOWN_R.push_back(sKeyFrameSprite(0.01f, 3));
	SYM_WALK_DOWN_R.push_back(sKeyFrameSprite(0.2f, 2));
	SYM_WALK_DOWN_R.push_back(sKeyFrameSprite(0.3f, 2));
	CreateSpriteAnimation(NPC, "SYM_WALK_DOWN_R", SYM_WALK_DOWN_R);

	std::vector<sKeyFrameSprite> SYM_WALK_LEFT_L;
	SYM_WALK_LEFT_L.push_back(sKeyFrameSprite(0.01f, 5));
	SYM_WALK_LEFT_L.push_back(sKeyFrameSprite(0.2f, 4));
	SYM_WALK_LEFT_L.push_back(sKeyFrameSprite(0.3f, 4));
	CreateSpriteAnimation(NPC, "SYM_WALK_LEFT_L", SYM_WALK_LEFT_L);

	std::vector<sKeyFrameSprite> SYM_WALK_LEFT_R;
	SYM_WALK_LEFT_R.push_back(sKeyFrameSprite(0.01f, 6));
	SYM_WALK_LEFT_R.push_back(sKeyFrameSprite(0.2f, 4));
	SYM_WALK_LEFT_R.push_back(sKeyFrameSprite(0.3f, 4));
	CreateSpriteAnimation(NPC, "SYM_WALK_LEFT_R", SYM_WALK_LEFT_R);

	std::vector<sKeyFrameSprite> SYM_WALK_RIGHT_L;
	SYM_WALK_RIGHT_L.push_back(sKeyFrameSprite(0.01f, 5, true));
	SYM_WALK_RIGHT_L.push_back(sKeyFrameSprite(0.2f, 4, true));
	SYM_WALK_RIGHT_L.push_back(sKeyFrameSprite(0.3f, 4, true));
	CreateSpriteAnimation(NPC, "SYM_WALK_RIGHT_L", SYM_WALK_RIGHT_L);

	std::vector<sKeyFrameSprite> SYM_WALK_RIGHT_R;
	SYM_WALK_RIGHT_R.push_back(sKeyFrameSprite(0.01f, 6, true));
	SYM_WALK_RIGHT_R.push_back(sKeyFrameSprite(0.2f, 4, true));
	SYM_WALK_RIGHT_R.push_back(sKeyFrameSprite(0.3f, 4, true));
	CreateSpriteAnimation(NPC, "SYM_WALK_RIGHT_R", SYM_WALK_RIGHT_R);

	std::vector<sKeyFrameSprite> OWP_WALK_UP;
	OWP_WALK_UP.push_back(sKeyFrameSprite(0.01f, 12));
	OWP_WALK_UP.push_back(sKeyFrameSprite(0.15f, 13));
	OWP_WALK_UP.push_back(sKeyFrameSprite(0.3f, 14));
	OWP_WALK_UP.push_back(sKeyFrameSprite(0.45f, 15));
	OWP_WALK_UP.push_back(sKeyFrameSprite(0.6f, 15));
	CreateSpriteAnimation(OW_POKEMON, "WALK_UP", OWP_WALK_UP);

	std::vector<sKeyFrameSprite> OWP_WALK_DOWN;
	OWP_WALK_DOWN.push_back(sKeyFrameSprite(0.01f, 0));
	OWP_WALK_DOWN.push_back(sKeyFrameSprite(0.15f, 1));
	OWP_WALK_DOWN.push_back(sKeyFrameSprite(0.3f, 2));
	OWP_WALK_DOWN.push_back(sKeyFrameSprite(0.45f, 3));
	OWP_WALK_DOWN.push_back(sKeyFrameSprite(0.6f, 3));
	CreateSpriteAnimation(OW_POKEMON, "WALK_DOWN", OWP_WALK_DOWN);

	std::vector<sKeyFrameSprite> OWP_WALK_LEFT;
	OWP_WALK_LEFT.push_back(sKeyFrameSprite(0.01f, 4));
	OWP_WALK_LEFT.push_back(sKeyFrameSprite(0.15f, 5));
	OWP_WALK_LEFT.push_back(sKeyFrameSprite(0.3f, 6));
	OWP_WALK_LEFT.push_back(sKeyFrameSprite(0.45f, 7));
	OWP_WALK_LEFT.push_back(sKeyFrameSprite(0.6f, 7));
	CreateSpriteAnimation(OW_POKEMON, "WALK_LEFT", OWP_WALK_LEFT);

	std::vector<sKeyFrameSprite> OWP_WALK_RIGHT;
	OWP_WALK_RIGHT.push_back(sKeyFrameSprite(0.01f, 8));
	OWP_WALK_RIGHT.push_back(sKeyFrameSprite(0.15f, 9));
	OWP_WALK_RIGHT.push_back(sKeyFrameSprite(0.3f, 10));
	OWP_WALK_RIGHT.push_back(sKeyFrameSprite(0.45f, 11));
	OWP_WALK_RIGHT.push_back(sKeyFrameSprite(0.6f, 11));
	CreateSpriteAnimation(OW_POKEMON, "WALK_RIGHT", OWP_WALK_RIGHT);
}

void cAnimationManager::AddAnimation(cAnimation* newAnimation)
{
	animations.push_back(newAnimation);
}

void cAnimationManager::RemoveAnimation(cAnimation* animationToRemove)
{
	for (int i = 0; i < animations.size(); i++)
	{
		if (animationToRemove == animations[i])
		{
			animations.erase(animations.begin() + i);
			return;
		}
	}
}

void cAnimationManager::CreateSpriteAnimation(eSpriteEntityType spriteType, std::string animationName, std::vector<sKeyFrameSprite>& keyframes)
{
	for (sEntitySpriteAnimationPreset presetData : entitySpriteAnimationPresets[spriteType])
	{
		if (presetData.animationName == animationName) return; // already exists
	}

	sEntitySpriteAnimationPreset newPresetData;
	newPresetData.animationName = animationName;
	newPresetData.keyframes = keyframes;
	entitySpriteAnimationPresets[spriteType].push_back(newPresetData);
}

bool cAnimationManager::GetSpriteAnimationKeyframes(eSpriteEntityType spriteType, std::string animationName, std::vector<sKeyFrameSprite>& keyframes)
{
	for (sEntitySpriteAnimationPreset presetData : entitySpriteAnimationPresets[spriteType])
	{
		if (presetData.animationName == animationName)
		{
			keyframes = presetData.keyframes;
			return true;
		}
	}

	return false;
}
