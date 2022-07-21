#pragma once

#include <set>
#include <map>
#include <string>
#include "cModel.h"

struct sCorrectionTiles
{
	std::vector<glm::vec3> walkableOffsets;
	std::vector<glm::vec3> unwalkableOffsets;
};

struct sTile
{
	bool isWalkable = false;
	//cEntity* entity;
	bool isUnchangeable = false;
};

struct sQuadrant
{
	sTile quadData[32][32][31];

	int quadX;
	int quadZ;
};

class cMapManager
{
	std::vector<sQuadrant> quads;
	std::map<int, sCorrectionTiles> walkableTiles;
	cModel* mapModel;

public:

	cMapManager();
	~cMapManager();

	void LoadMap(std::string mapModelName, std::string mapDescName);
	int MoveEntity(glm::vec3 currPosition, int direction);
};