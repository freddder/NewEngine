#include "cMapManager.h"

#include <sstream>
#include <fstream>

#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>

#include "cFloatAnimation.h"
#include "cRenderManager.h"
#include "cAnimationManager.h"
#include "cAnimatedModel.h"

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
}

cMapManager::~cMapManager()
{	
	//delete mapModel;
	cRenderManager::GetInstance()->RemoveModel(mapModel);

	for (std::map<int, sInstancedTile>::iterator it = instancedTiles.begin(); it != instancedTiles.end(); it++)
	{
		cRenderManager::GetInstance()->RemoveModel(it->second.instancedModel);
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
		mapModel = cRenderManager::CreateRenderModel();
		mapModel->position = glm::vec3(0.5f, 0.f, 0.5f);
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

		rapidjson::Value& tileIds = correctionTileData[i]["tileId"];
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

		int animationType = currInstancedTile["animationType"].GetInt();
		instancedTiles[tileId].instancedModel = cRenderManager::CreateAnimatedModel(static_cast<eAnimatedModel>(animationType));
		instancedTiles[tileId].instancedModel->meshName = currInstancedTile["meshName"].GetString();
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
		quads.emplace_back();
		sQuadrant& newQuad = quads.back();
		int tempLayers[32][32][8];

		// set quadrant coords
		pdsmap >> newQuad.posX;
		pdsmap >> newQuad.posZ;

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

					int tileId = tempLayers[x][z][layerId];

					if (tileId == -1) continue;

					sTile& currTile = newQuad.data[ToTileIndex(x, z, currHeight)];

					if (walkableTiles.find(tileId) != walkableTiles.end() && !currTile.isUnchangeable) // is walkable
					{						
						currTile.isWalkable = true;

						// Walkable correction tiles
						for (unsigned int i = 0; i < walkableTiles[tileId].walkableOffsets.size(); i++)
						{
							int correctionX = x + (int)walkableTiles[tileId].walkableOffsets[i].x;
							int correctionZ = z + (int)walkableTiles[tileId].walkableOffsets[i].z;
							int correctionHeight = currHeight + (int)walkableTiles[tileId].walkableOffsets[i].y;

							newQuad.data[ToTileIndex(correctionX, correctionZ, correctionHeight)].isWalkable = true;
						}

						// Unwalkable correction tiles
						for (unsigned int i = 0; i < walkableTiles[tileId].unwalkableOffsets.size(); i++)
						{
							int correctionX = x + (int)walkableTiles[tileId].unwalkableOffsets[i].x;
							int correctionZ = z + (int)walkableTiles[tileId].unwalkableOffsets[i].z;
							int correctionHeight = currHeight + (int)walkableTiles[tileId].unwalkableOffsets[i].y;

							newQuad.data[ToTileIndex(correctionX, correctionZ, correctionHeight)].isWalkable = false;
							newQuad.data[ToTileIndex(correctionX, correctionZ, correctionHeight)].isUnchangeable = true;
						}
					}
					else // is NOT walkable
					{
						currTile.isWalkable = false;
						currTile.isUnchangeable = true;
					}

					if (instancedTiles.find(tileId) != instancedTiles.end()) // it exists
					{
						glm::vec4 newOffset = glm::vec4((newQuad.posX * 32 - 15 + x), currHeight, (newQuad.posX * 32 - 15 + z), 1.f);
						newOffset.x += instancedTiles[tileId].modelOffset.x;
						newOffset.y += instancedTiles[tileId].modelOffset.y;
						newOffset.z += instancedTiles[tileId].modelOffset.z;

						instancedTiles[tileId].instanceOffsets.push_back(newOffset);
					}
				}
			}
		}

		pdsmap >> currToken; // this should be mapend
		pdsmap >> currToken; // if there is another quad, this will be mapstart
	}
	// end here

	// Load tile specific animations
	for (std::map<int, sInstancedTile>::iterator it = instancedTiles.begin(); it != instancedTiles.end(); it++)
	{
		if (it->second.instanceOffsets.size() != 0)
		{
			it->second.instancedModel->InstanceObject(it->second.instanceOffsets, cRenderManager::GetInstance()->GetCurrentShaderId());
			cAnimationManager::AddAnimation(it->second.instancedModel->animation);

			it->second.instanceOffsets.clear();
		}
	}

	pdsmap.close();
}

int cMapManager::TryMoveEntity(glm::vec3 currPosition, eDirection direction)
{
	int desiredX = currPosition.x;
	int desiredZ = currPosition.z;

	if (direction == eDirection::UP)
		desiredX += 1;
	else if (direction == eDirection::DOWN)
		desiredX -= 1;
	else if (direction == eDirection::LEFT)
		desiredZ -= 1;
	else if (direction == eDirection::RIGHT)
		desiredZ += 1;

	if (desiredX + 15 < 0 || desiredZ + 15 < 0) return 0;

	desiredX += 15;
	desiredZ += 15;

	int quadXCoord = desiredX / 32;
	int quadZCoord = desiredZ / 32;

	for (std::vector<sQuadrant>::iterator it = quads.begin(); it != quads.end(); it++)
	{
		if (quadXCoord == it->posX && quadZCoord == it->posZ)
		{
			desiredZ -= 32 * quadZCoord;
			desiredX -= 32 * quadXCoord;

			if (it->data[ToTileIndex(desiredX, desiredZ, (int)currPosition.y)].isWalkable) // same height
				return 1;
			else if (it->data[ToTileIndex(desiredX, desiredZ, (int)currPosition.y + 1)].isWalkable) // go up
				return 2;
			else if (it->data[ToTileIndex(desiredX, desiredZ, (int)currPosition.y - 1)].isWalkable) // go down
				return 3;

			return 0;
		}
	}

	return 0;
}

int cMapManager::ToTileIndex(int x, int z, int height)
{
	if (x > 31 || x < 0
		|| z > 31 || z < 0
		|| height > 15 || height < -15) return -1;

	int heightIndex = (height + 15) * (32 * 32);
	int layerIndex = 32 * x + z;

	return heightIndex + layerIndex;
}
