#pragma once

#include <set>
#include <map>
#include <string>
#include <memory>
#include "cAnimatedModel.h"
#include "cEntity.h"

struct sInstancedTile
{
	std::shared_ptr<cAnimatedModel> instancedModel;
	std::vector<glm::vec4> instanceOffsets;
	glm::vec3 modelOffset;
};

struct sCorrectionTiles
{
	std::vector<glm::vec3> walkableOffsets;
	std::vector<glm::vec3> unwalkableOffsets;
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

	sTile* GetTileFromLocalPosition(glm::vec3 localPos);
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
	static cMapManager* singleton;
	cMapManager();
	~cMapManager();
	cMapManager(const cMapManager& obj) = delete;
public:
	static cMapManager* GetInstance()
	{
		if (singleton == NULL)
		{
			singleton = new cMapManager();
		}

		return singleton;
	}
	static void DestroyInstance()
	{
		if (singleton != NULL)
		{
			delete singleton;
			singleton = NULL;
		}
	}

private:
	std::vector<sQuadrant> quads;
	std::map<int, sCorrectionTiles> walkableTiles;
	std::shared_ptr<cRenderModel> mapModel;
	std::map<int, sInstancedTile> instancedTiles;
	sQuadrant* GetQuad(int worldX, int worldZ);
public:
	void LoadMap(std::string mapDescriptionFile);

	eEntityMoveResult TryMoveEntity(cEntity* entityToMove, eDirection direction);
};