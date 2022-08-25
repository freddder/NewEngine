#pragma once

#include <set>
#include <map>
#include <string>
#include "cWaveModel.h"
#include "cOceanModel.h"
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
	bool isWalkable = false;
	//cEntity* entity;
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
	std::vector<sQuadrant> quads;
	std::map<int, sCorrectionTiles> walkableTiles;
	cRenderModel* mapModel;

	std::map<int, sInstancedTile> instancedTiles;
	//cModel* Water_b;
	//cModel* Water_c;
	//cModel* Water_bl;

	//cSinAnimation* water_b_animation;
	//cSinAnimation* water_c_animation;
	//cSinAnimation* water_bl_animation;

public:

	cMapManager();
	~cMapManager();

	void LoadMap(std::string mapModelName, std::string mapDescName);
	int MoveEntity(glm::vec3 currPosition, int direction);
};