#pragma once

#include <set>
#include <string>
#include "cModel.h"

struct sTile
{
	bool isWalkable;
	//cEntity* entity;
};

struct sQuadrant
{
	sTile quadData[32][32][31];

	int quadX;
	int quadZ;
};

class cMapManager
{
	std::set<sQuadrant> quads;
	cModel* mapModel;

public:

	cMapManager();
	~cMapManager();

	void LoadMap(std::string mapModelName, std::string mapDescName);
	//void MoveEntity(cEntity* entity, eDirection dir);
};