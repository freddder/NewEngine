#pragma once

#include "cSpriteModel.h"
#include "cAnimation.h"
#include <memory>

enum eEntityMoveResult;

class cCharacterSprite
{
public:
	cCharacterSprite(std::string textureName, glm::vec3 pos);
	virtual ~cCharacterSprite();

protected:
	std::shared_ptr<cSpriteAnimation> spriteAnimation;
	std::shared_ptr<cModelAnimation> modelAnimation;
public:
	std::shared_ptr<cSpriteModel> model;
	virtual glm::vec3 AnimateMovement(eDirection dir, bool run, eEntityMoveResult moveResult);

	friend class cCharacterEntity;
};

class cOverworldPokemonSprite : public cCharacterSprite
{
public:
	cOverworldPokemonSprite(std::string textureName, glm::vec3 pos);
	virtual ~cOverworldPokemonSprite();

private:
	eDirection lastDesiredDirection;
public:
	virtual glm::vec3 AnimateMovement(eDirection dir, bool run, eEntityMoveResult moveResult);
};

class cNPCSprite : public cCharacterSprite
{
public:
	cNPCSprite(std::string textureName, glm::vec3 pos);
	virtual ~cNPCSprite();

private:
	bool switchLeg;
public:
	virtual glm::vec3 AnimateMovement(eDirection dir, bool run, eEntityMoveResult moveResult);
};

class cPlayerSprite : public cCharacterSprite
{
public:
	cPlayerSprite(std::string textureName, glm::vec3 pos);
	virtual ~cPlayerSprite();

private:
	eDirection lastDesiredDirection;
	bool switchLeg;
	void SetupSpriteWalk(eDirection dir);
	void SetupSpriteRun(eDirection dir);
public:
	virtual glm::vec3 AnimateMovement(eDirection dir, bool run, eEntityMoveResult moveResult);
	void StopMovement();
};

class cBattleSprite
{
public:
	cBattleSprite(glm::vec3 pos);
	~cBattleSprite();

	void SetSpriteData(std::string textureName, float spriteHeightSize, float spriteAspectRatio, int spritesNum);
	void ClearSpriteData();

protected:
	std::shared_ptr<cPeriodicSpriteAnimation> spriteAnimation;
	std::shared_ptr<cSpriteModel> model;
};