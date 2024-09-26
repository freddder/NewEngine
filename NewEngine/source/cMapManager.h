#pragma once

#include <set>
#include <map>
#include <string>
#include <memory>
#include "cAnimatedModel.h"
#include "cEntity.h"
#include "CharacterSprite.h"

struct sInstancedTile
{
	std::shared_ptr<cAnimatedModel> instancedModel;
	std::vector<glm::vec4> instanceOffsets;
	glm::vec3 modelOffset;
};

struct sCorrectionTiles
{
	std::vector<glm::ivec3> walkableOffsets;
	std::vector<glm::ivec3> unwalkableOffsets;
};

struct sTile
{
	cEntity* entity = nullptr;
	bool isWalkable = false;
	bool isUnchangeable = false;

	// For walking or spawning
	bool IsAvailable()
	{
		if (!isWalkable) return false;

		if (entity && entity->blocksTile) return false;

		return true;
	}
};

struct sQuadrant
{
	int posX;
	int posZ;

	std::map<int, sTile> data;

	int wildPokemonCount = 0;
	std::vector<int> localSpawnTiles;
	sTile* GetRandomSpawnTile(glm::vec3& globalPos);
	
	static int GetTileIdFromPosition(glm::ivec3 localPos);
	sTile* GetTileFromLocalPosition(glm::ivec3 localPos);

	glm::vec3 LocalPositionToGlobalPosition(glm::ivec3 localPos);
	glm::vec3 TileIdToGlobalPosition(int tileId);
};

enum eEntityMoveResult
{
	FAILURE,
	SUCCESS,
	SUCCESS_UP,
	SUCCESS_DOWN
};

class cMapManager
{
public:
	cMapManager();
	~cMapManager();

	void Startup();
	void Shutdown();

private:
	std::vector<sQuadrant> quads;
	std::map<int, sCorrectionTiles> walkableTiles;
	std::shared_ptr<cRenderModel> mapModel;
	std::shared_ptr<cRenderModel> arenaModel;
	std::map<int, sInstancedTile> mapInstancedTiles;
	std::map<int, sInstancedTile> arenaInstancedTiles;
	sQuadrant* GetQuad(int worldX, int worldZ);
	void LoadArena(const std::string arenaDescriptionFile);
public:
	void LoadScene(const std::string mapDescriptionFile);

public:
	sTile* GetTile(glm::ivec3 worldPosition);
	sTile* GetRandomSpawnTile(glm::vec3& globalPositionOut);
	void RemoveEntityFromTile(glm::ivec3 worldPosition);

	eEntityMoveResult TryMoveEntity(cEntity* entityToMove, eDirection direction);
	
	cBattleSprite* opponentSpriteModel;
	cBattleSprite* playerSpriteModel;
};