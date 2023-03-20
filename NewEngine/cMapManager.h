#pragma once

#include <set>
#include <map>
#include <string>
#include "cWaveModel.h"
#include "cOceanModel.h"
#include "cFoamModel.h"
#include "cSinAnimation.h"

struct sInstancedTile
{
	cRenderModel* instancedModel;
	std::vector<glm::vec4> instanceOffsets;
	glm::vec3 modelOffset;
	cAnimation* animation;
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
	//sTile quadData[32][32][31];

	int quadX;
	int quadZ;

	std::map<int, std::map<
		int, std::map<
		int, sTile>>> quadData;
};

class cMapManager
{
	static cMapManager* singleton;
	cMapManager();
	~cMapManager();
	cMapManager(const cMapManager& obj) = delete;

	std::vector<sQuadrant> quads;
	std::map<int, sCorrectionTiles> walkableTiles;
	cRenderModel* mapModel;

	std::map<int, sInstancedTile> instancedTiles;

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

	void LoadMap(std::string mapDescriptionFile);
	int TryMoveEntity(glm::vec3 currPosition, int direction);
};