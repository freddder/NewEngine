#include "cMapManager.h"

#include <sstream>
#include <fstream>

#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>

#include "cRenderManager.h"
#include "cAnimationManager.h"
#include "cAnimatedModel.h"
#include "cSceneManager.h"

#include "Player.h"
#include "cPlayerEntity.h"

cMapManager* cMapManager::sgtn = NULL;

int sQuadrant::GetTileIdFromPosition(glm::vec3 localPos)
{
	if (localPos.x > 31 || localPos.x < 0 ||
		localPos.z > 31 || localPos.z < 0 ||
		localPos.y > 15 || localPos.y < -15) return -1;

	int heightIndex = ((int)localPos.y + 15) * (32 * 32);
	int layerIndex = 32 * (int)localPos.x + (int)localPos.z;

	return heightIndex + layerIndex;
}

sTile* sQuadrant::GetTileFromLocalPosition(glm::vec3 localPos)
{
	int localTileId = GetTileIdFromPosition(localPos);

	if (localTileId == -1) return nullptr;

	return &data[localTileId];
}

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

sQuadrant* cMapManager::GetQuad(int worldX, int worldZ)
{
	if (worldX + 15 < 0 || worldZ + 15 < 0) return nullptr;

	worldX += 15;
	worldZ += 15;

	int quadXCoord = worldX / 32;
	int quadZCoord = worldZ / 32;

	for (int i = 0; i < quads.size(); i++)
	{
		if (quadXCoord == quads[i].posX && quadZCoord == quads[i].posZ) return &quads[i];
	}

	return nullptr;
}

void cMapManager::LoadMap(std::string mapDescriptionFile)
{
	// Load json info file with rapidjson
	FILE* fp = 0;
	fopen_s(&fp, ("assets/maps/" + mapDescriptionFile).c_str(), "rb"); // non-Windows use "r"

	// OPTIMIZATION: best buffer size might be different
	char readBuffer[4096];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	rapidjson::Document d;
	d.ParseStream(is);

	if (fp == 0) return;
	fclose(fp);

	// unload old map (how to unload textures?)
	
	// Load new map
	std::string mapModelName = d["mapModelFileName"].GetString();
	cRenderManager::GetInstance()->LoadModel(mapModelName, "scene");

	// Create map model
	if (!mapModel)
	{
		mapModel = cRenderManager::CreateRenderModel();
		mapModel->position = glm::vec3(0.5f, 0.f, 0.5f);
	}

	mapModel->meshName = mapModelName;

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

	// Load spawn data
	std::vector<int> spawnTileIds;
	if (d.HasMember("wildSpawning"))
	{
		rapidjson::Value& spawnData = d["wildSpawning"];
		for (unsigned int i = 0; i < spawnData.Size(); i++)
		{
			Pokemon::eSpawnType spawnType = static_cast<Pokemon::eSpawnType>(spawnData[i]["spawnType"].GetInt());
			if (spawnType == Pokemon::TALL_GRASS)
			{
				rapidjson::Value& spawnTileId = spawnData[i]["spawnTileId"];
				for (int j = 0; j < spawnTileId.Size(); j++)
				{
					spawnTileIds.push_back(spawnTileId[j].GetInt());
				}
			}
		}
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

					sTile* currTile = newQuad.GetTileFromLocalPosition(glm::vec3(x, currHeight, z));

					if (!currTile->isUnchangeable && walkableTiles.find(tileId) != walkableTiles.end()) // is walkable
					{						
						currTile->isWalkable = true;

						// Walkable correction tiles
						for (unsigned int i = 0; i < walkableTiles[tileId].walkableOffsets.size(); i++)
						{
							int correctionX = x + (int)walkableTiles[tileId].walkableOffsets[i].x;
							int correctionZ = z + (int)walkableTiles[tileId].walkableOffsets[i].z;
							int correctionHeight = currHeight + (int)walkableTiles[tileId].walkableOffsets[i].y;

							newQuad.GetTileFromLocalPosition(glm::vec3(correctionX, correctionHeight, correctionZ))->isWalkable = true;
						}

						// Unwalkable correction tiles
						for (unsigned int i = 0; i < walkableTiles[tileId].unwalkableOffsets.size(); i++)
						{
							int correctionX = x + (int)walkableTiles[tileId].unwalkableOffsets[i].x;
							int correctionZ = z + (int)walkableTiles[tileId].unwalkableOffsets[i].z;
							int correctionHeight = currHeight + (int)walkableTiles[tileId].unwalkableOffsets[i].y;

							sTile* tileToCorrect = newQuad.GetTileFromLocalPosition(glm::vec3(correctionX, correctionHeight, correctionZ));
							tileToCorrect->isWalkable = false;
							tileToCorrect->isUnchangeable = true;
						}

						if (std::find(spawnTileIds.begin(), spawnTileIds.end(), tileId) != spawnTileIds.end())
						{
							newQuad.localSpawnTiles.push_back(sQuadrant::GetTileIdFromPosition(glm::vec3(x, currHeight, z)));
						}
					}
					else // is NOT walkable
					{
						currTile->isWalkable = false;
						currTile->isUnchangeable = true;
					}

					if (instancedTiles.find(tileId) != instancedTiles.end()) // it exists
					{
						glm::vec4 newOffset = glm::vec4((newQuad.posX * 32 - 15 + x), currHeight, (newQuad.posZ * 32 - 15 + z), 1.f);
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

			if (it->second.instancedModel->animation)
				cAnimationManager::AddAnimation(it->second.instancedModel->animation);

			it->second.instanceOffsets.clear();
		}
	}

	pdsmap.close();
}

sTile* cMapManager::GetTile(glm::vec3 worldPosition)
{
	if (sQuadrant* quad = GetQuad((int)worldPosition.x, (int)worldPosition.z))
	{
		// TODO: probably a good idea to make a world to local function
		glm::vec3 localPosition = worldPosition;
		localPosition.x += 15 - 32 * quad->posX;
		localPosition.z += 15 - 32 * quad->posZ;

		return quad->GetTileFromLocalPosition(localPosition);
	}

	return nullptr;
}

sTile* cMapManager::GetRandomSpawnTile(glm::vec3& globalPositionOut)
{
	// Semi random: make sure to pick a tile close to player

	glm::vec3 playerPosition = Player::GetPlayerPosition();
	int quadPosX = (playerPosition.x + 15) / 32;
	int quadPosZ = (playerPosition.z + 15) / 32;

	sQuadrant* playerQuad = GetQuad(quadPosX, quadPosZ);
	
	

	return nullptr;
}

eEntityMoveResult cMapManager::TryMoveEntity(cEntity* entityToMove, eDirection direction)
{
	int desiredPosX = (int)entityToMove->position.x;
	int desiredPosY = (int)entityToMove->position.y;
	int desiredPosZ = (int)entityToMove->position.z;

	if (direction == eDirection::UP)
		desiredPosX += 1;
	else if (direction == eDirection::DOWN)
		desiredPosX -= 1;
	else if (direction == eDirection::LEFT)
		desiredPosZ -= 1;
	else if (direction == eDirection::RIGHT)
		desiredPosZ += 1;

	sQuadrant* desiredQuad = GetQuad(desiredPosX, desiredPosZ);
	if (!desiredQuad) return eEntityMoveResult::FAILURE; // Tries to walk on a quad that doesn't exists

	desiredPosX += 15 - 32 * desiredQuad->posX;
	desiredPosZ += 15 - 32 * desiredQuad->posZ;

	eEntityMoveResult moveResult = eEntityMoveResult::FAILURE;
	if (desiredQuad->GetTileFromLocalPosition(glm::vec3(desiredPosX, desiredPosY, desiredPosZ))->IsAvailable()) // same height
	{
		moveResult = eEntityMoveResult::SUCCESS;
	}
	else if (desiredQuad->GetTileFromLocalPosition(glm::vec3(desiredPosX, desiredPosY + 1, desiredPosZ))->IsAvailable()) // go up
	{
		moveResult = eEntityMoveResult::SUCCESS_UP;
		desiredPosY++;
	}
	else if (desiredQuad->GetTileFromLocalPosition(glm::vec3(desiredPosX, desiredPosY - 1, desiredPosZ))->IsAvailable()) // go down
	{
		moveResult = eEntityMoveResult::SUCCESS_DOWN;
		desiredPosY--;
	}

	if (moveResult != eEntityMoveResult::FAILURE)
	{

		// OPTIMIZATION: it might be better to have a check on position x and z and direction to easily check
		// if entity will be changing quad and not have to search for quads twice with a single function call
		if (sQuadrant* currQuad = GetQuad((int)entityToMove->position.x, (int)entityToMove->position.z))
		{
			int currLocalX = (int)entityToMove->position.x + 15 - 32 * currQuad->posX;
			int currLocalZ = (int)entityToMove->position.z + 15 - 32 * currQuad->posZ;
			if (sTile* currTile = currQuad->GetTileFromLocalPosition(glm::vec3(currLocalX, entityToMove->position.y, currLocalZ)))
			{
				if (currTile->entity == entityToMove)
				{
					currTile->entity = nullptr;
				}
			}
		}

		sTile* tileToWalk = desiredQuad->GetTileFromLocalPosition(glm::vec3(desiredPosX, desiredPosY, desiredPosZ));
		if (tileToWalk->entity)
		{
			if (dynamic_cast<cPlayerEntity*>(entityToMove) == Player::playerChar) // Player walk into it
			{
				tileToWalk->entity->WalkInteract();
			}
			else if (dynamic_cast<cPlayerEntity*>(tileToWalk->entity) == Player::playerChar) // It walk into player
			{
				entityToMove->WalkInteract();
			}
		}
		else
		{
			tileToWalk->entity = entityToMove;
		}
	}

	return moveResult;
}
