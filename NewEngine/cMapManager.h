#pragma once

#include <set>
#include <string>
#include "cModel.h"

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
	std::set<int> walkableTiles;
	cModel* mapModel;

public:

	cMapManager();
	~cMapManager();

	void LoadMap(std::string mapModelName, std::string mapDescName);
	int MoveEntity(glm::vec3 currPosition, int direction);
};