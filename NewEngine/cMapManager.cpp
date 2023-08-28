#include "cMapManager.h"

#include <sstream>
#include <fstream>

#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>

#include "cFloatAnimation.h"
#include "cRenderManager.h"
#include "cAnimationManager.h"
#include "cOceanModel.h"
#include "cFoamModel.h"
#include "cWaveModel.h"

cMapManager* cMapManager::singleton = NULL;

cMapManager::cMapManager()
{
	//	^ x
	//  > z
	
	// grass - 100,101,103,52,53,54,55,56,57,58,59,92,95

	// snow - 191

	// left stair5 - 148
	// mid stair5 - 149
	// left stair5 - 150

	// left stair - 152
	// mid stair - 153
	// right stair - 154

	//walkableTiles[184].walkableOffsets.push_back(glm::vec3(1.f, 0.f, 0.f));
	//walkableTiles[184].walkableOffsets.push_back(glm::vec3(2.f, 0.f, 0.f));
}

cMapManager::~cMapManager()
{	
	delete mapModel;

	for (std::map<int, sInstancedTile>::iterator it = instancedTiles.begin(); it != instancedTiles.end(); it++)
	{
		delete it->second.instancedModel;
	}
}

void cMapManager::LoadMap(std::string mapDescriptionFile)
{
	// Load json info file with rapidjson
	FILE* fp = 0;
	fopen_s(&fp, ("assets/maps/" + mapDescriptionFile).c_str(), "rb"); // non-Windows use "r"

	char readBuffer[65536];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	rapidjson::Document d;
	d.ParseStream(is);

	if (fp == 0) return;
	fclose(fp);

	// unload old map (how to unload textures?)
	// load new map
	// update model mesh name

	// Load map model
	if (!mapModel)
	{
		mapModel = new cRenderModel();
		mapModel->position = glm::vec3(0.5f, 0.f, 0.5f);
		cRenderManager::GetInstance()->AddModel(mapModel);
	}

	mapModel->meshName = d["mapModelFileName"].GetString();

	// Load simple walkable tiles
	rapidjson::Value& walkableTileData = d["walkableTiles"];
	for (unsigned int i = 0; i < walkableTileData.Size(); i++)
	{
		walkableTiles[walkableTileData[i].GetInt()];
	}

	// Load complex walkable tiles
	rapidjson::Value& correctionTileData = d["correctionTiles"];
	for (unsigned int i = 0; i < correctionTileData.Size(); i++)
	{
		//int tileId = correctionTileData[i]["tileId"].GetInt();

		std::vector<glm::vec3> newTileWalkableOffsets;
		std::vector<glm::vec3> newTileUnwalkableOffsets;

		rapidjson::Value& walkableData = correctionTileData[i]["walkableTiles"];
		for (unsigned int j = 0; j < walkableData.Size(); j++)
		{
			glm::vec3 newWalkableOffset;
			newWalkableOffset.x = walkableData[j]["x"].GetFloat();
			newWalkableOffset.y = walkableData[j]["y"].GetFloat();
			newWalkableOffset.z = walkableData[j]["z"].GetFloat();
			newTileWalkableOffsets.push_back(newWalkableOffset);
		}

		rapidjson::Value& unwalkableData = correctionTileData[i]["unwalkableTiles"];
		for (unsigned int j = 0; j < unwalkableData.Size(); j++)
		{
			glm::vec3 newUnwalkableOffset;
			newUnwalkableOffset.x = unwalkableData[j]["x"].GetFloat();
			newUnwalkableOffset.y = unwalkableData[j]["y"].GetFloat();
			newUnwalkableOffset.z = unwalkableData[j]["z"].GetFloat();
			newTileUnwalkableOffsets.push_back(newUnwalkableOffset);
		}

		rapidjson::Value& tileIds = correctionTileData[i]["tileIds"];
		for (unsigned int j = 0; j < tileIds.Size(); j++)
		{
			int tileId = tileIds[j].GetInt();
			walkableTiles[tileId].walkableOffsets = newTileWalkableOffsets;
			walkableTiles[tileId].unwalkableOffsets = newTileUnwalkableOffsets;
		}
	}

	// Load tile animations
	rapidjson::Value& instancedTileData = d["instancedTiles"];

	for (unsigned int i = 0; i < instancedTileData.Size(); i++)
	{
		rapidjson::Value& currInstancedTile = d["instancedTiles"][i];

		int tileId = currInstancedTile["tileId"].GetInt();

		float meshOrientationY = currInstancedTile["meshYOrientation"].GetFloat();
		glm::vec3 meshPosOffset;
		meshPosOffset.x = currInstancedTile["meshOffset"]["x"].GetFloat();
		meshPosOffset.y = currInstancedTile["meshOffset"]["y"].GetFloat();
		meshPosOffset.z = currInstancedTile["meshOffset"]["z"].GetFloat();

		std::string animationType = currInstancedTile["animationType"].GetString();
		if (animationType == "ocean")
		{
			instancedTiles[tileId].instancedModel = new cOceanModel();
			instancedTiles[tileId].instancedModel->meshName = currInstancedTile["meshName"].GetString();
		}
		else if (animationType == "wave")
		{
			instancedTiles[tileId].instancedModel = new cWaveModel();
			instancedTiles[tileId].instancedModel->meshName = currInstancedTile["meshName"].GetString();
		}
		else if (animationType == "foam")
		{
			instancedTiles[tileId].instancedModel = new cFoamModel();
			instancedTiles[tileId].instancedModel->meshName = currInstancedTile["meshName"].GetString();
		}

		instancedTiles[tileId].instancedModel->orientation.y = glm::radians(meshOrientationY);
		instancedTiles[tileId].modelOffset = meshPosOffset;
	}

	// Load collision map
	std::string collisionMapFileName = d["mapCollisionFileName"].GetString();
	std::ifstream pdsmap("assets/maps/" + collisionMapFileName);

	if (!pdsmap.is_open()) return;

	std::string currToken;

	// make sure reader is at first mapstart
	while (pdsmap >> currToken)
	{
		if (currToken == "mapstart") break;
	}

	// start here
	while (currToken == "mapstart")
	{
		sQuadrant newQuad;
		int tempLayers[32][32][8];

		// set quadrant coords
		pdsmap >> newQuad.quadZ;
		pdsmap >> newQuad.quadX;

		// skip areaindex
		pdsmap >> currToken;
		pdsmap >> currToken;

		for (int layerId = 0; layerId < 8; layerId++)
		{
			pdsmap >> currToken;

			if (currToken != "tilegrid") break;

			for (int x = 0; x < 32; x++)
			{
				for (int z = 0; z < 32; z++)
				{
					pdsmap >> tempLayers[x][z][layerId];
				}
			}
		}

		for (int layerId = 0; layerId < 8; layerId++)
		{
			pdsmap >> currToken;

			if (currToken != "heightgrid") break;

			for (int x = 0; x < 32; x++)
			{
				for (int z = 0; z < 32; z++)
				{
					int currHeight;
					pdsmap >> currHeight;

					int currTile = tempLayers[x][z][layerId];

					if (walkableTiles.find(currTile) != walkableTiles.end()) // contains
					{
						if (!newQuad.quadData[x][z][currHeight].isUnchangeable)
						{
							newQuad.quadData[x][z][currHeight].isWalkable = true;

							for (unsigned int i = 0; i < walkableTiles[currTile].walkableOffsets.size(); i++)
							{
								newQuad.quadData[x + (int)walkableTiles[currTile].walkableOffsets[i].x]
												[z + (int)walkableTiles[currTile].walkableOffsets[i].z]
												[currHeight + (int)walkableTiles[currTile].walkableOffsets[i].y].isWalkable = true;
							}

							for (unsigned int i = 0; i < walkableTiles[currTile].unwalkableOffsets.size(); i++)
							{
								newQuad.quadData[(x + (int)walkableTiles[currTile].unwalkableOffsets[i].x)]
												[(z + (int)walkableTiles[currTile].unwalkableOffsets[i].z)]
												[(currHeight + (int)walkableTiles[currTile].unwalkableOffsets[i].y)].isWalkable = false;

								newQuad.quadData[(x + (int)walkableTiles[currTile].unwalkableOffsets[i].x)]
												[(z + (int)walkableTiles[currTile].unwalkableOffsets[i].z)]
												[(currHeight + (int)walkableTiles[currTile].unwalkableOffsets[i].y)].isUnchangeable = true;
							}
						}
					}
					else if (tempLayers[x][z][layerId] != -1) // desn't contain
					{
						newQuad.quadData[x][z][currHeight].isWalkable = false;
						newQuad.quadData[x][z][currHeight].isUnchangeable = true;
					}

					if (instancedTiles.find(currTile) != instancedTiles.end()) // it exists
					{
						glm::vec4 newOffset = glm::vec4((newQuad.quadX * 32 - 15 + x), currHeight, (newQuad.quadX * 32 - 15 + z), 1.f);
						newOffset.x += instancedTiles[currTile].modelOffset.x;
						newOffset.y += instancedTiles[currTile].modelOffset.y;
						newOffset.z += instancedTiles[currTile].modelOffset.z;

						instancedTiles[currTile].instanceOffsets.push_back(newOffset);
					}
				}
			}
		}

		quads.push_back(newQuad);

		pdsmap >> currToken; // this should be mapend
		pdsmap >> currToken; // if there is another map, this will be mapstart
	}
	// end here

	// Load tile specific animations
	for (std::map<int, sInstancedTile>::iterator it = instancedTiles.begin(); it != instancedTiles.end(); it++)
	{
		if (it->second.instanceOffsets.size() != 0)
		{
			it->second.instancedModel->InstanceObject(it->second.instanceOffsets, cRenderManager::GetInstance()->GetCurrentShaderId());
			cAnimationManager::GetInstance()->AddAnimation(it->second.instancedModel->animation);
			cRenderManager::GetInstance()->AddModel(it->second.instancedModel);

			it->second.instanceOffsets.clear();
		}
	}

	pdsmap.close();
}

int cMapManager::TryMoveEntity(glm::vec3 currPosition, eDirection direction)
{
	glm::vec3 desiredLocation = currPosition;

	if (direction == eDirection::UP)
		desiredLocation.x += 1.f;
	else if (direction == eDirection::DOWN)
		desiredLocation.x -= 1.f;
	else if (direction == eDirection::LEFT)
		desiredLocation.z -= 1.f;
	else if (direction == eDirection::RIGHT)
		desiredLocation.z += 1.f;

	if (desiredLocation.x + 15 < 0 || desiredLocation.z + 15 < 0) return 0;

	desiredLocation.x += 15;
	desiredLocation.z += 15;

	int quadXCoord = ((int)desiredLocation.x) / 32;
	int quadZCoord = ((int)desiredLocation.z) / 32;

	for (std::vector<sQuadrant>::iterator it = quads.begin(); it != quads.end(); it++)
	{
		if (quadXCoord == it->quadX && quadZCoord == it->quadZ)
		{
			desiredLocation.z -= 32 * quadZCoord;
			desiredLocation.x -= 32 * quadXCoord;

			if (it->quadData[(int)desiredLocation.x][(int)desiredLocation.z][(int)desiredLocation.y].isWalkable) // same height
				return 1;
			else if (it->quadData[(int)desiredLocation.x][(int)desiredLocation.z][(int)desiredLocation.y + 1].isWalkable) // go up
				return 2;
			else if (it->quadData[(int)desiredLocation.x][(int)desiredLocation.z][(int)desiredLocation.y - 1].isWalkable) // go down
				return 3;

			return 0;
		}
	}

	return 0;
}