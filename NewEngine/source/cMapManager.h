#pragma once

#include <set>
#include <map>
#include <string>
#include <memory>
#include "cAnimatedModel.h"

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
	//cEntity* entity;
	bool isWalkable = false;
	bool isUnchangeable = false;
};

struct sQuadrant
{
	int posX;
	int posZ;

	std::map<int, sTile> data;
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
	int ToTileIndex(int x, int z, int height);
public:
	void LoadMap(std::string mapDescriptionFile);

	int TryMoveEntity(glm::vec3 currPosition, eDirection direction);
};