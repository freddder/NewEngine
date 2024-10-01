#include "cMapManager.h"

#include <sstream>
#include <fstream>

#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>

#include "cAnimatedModel.h"

#include "Player.h"
#include "cPlayerEntity.h"

#include "Engine.h"
#include "cRenderManager.h"
#include "cAnimationManager.h"
#include "cSceneManager.h"

#include "cTamedRoamingPokemon.h"
#include <iostream>

const std::string MAPS_PATH = "assets/scenes/maps/";
const std::string ARENAS_PATH = "assets/scenes/arenas/";

sTile* sQuadrant::GetRandomSpawnTile(glm::vec3& globalPos)
{
	if (wildPokemonCount >= 5) return nullptr;

	int tileId = localSpawnTiles[rand() % localSpawnTiles.size()];

	sTile* spawnTile = &data[tileId];
	globalPos = TileIdToGlobalPosition(tileId);
	return spawnTile;
}

int sQuadrant::GetTileIdFromPosition(glm::ivec3 localPos)
{
	if (localPos.x > 31 || localPos.x < 0 ||
		localPos.z > 31 || localPos.z < 0 ||
		localPos.y > 15 || localPos.y < -15) return -1;

	int heightIndex = (localPos.y + 15) * (32 * 32);
	int layerIndex = 32 * localPos.x + localPos.z;

	return heightIndex + layerIndex;
}

sTile* sQuadrant::GetTileFromLocalPosition(glm::ivec3 localPos)
{
	int localTileId = GetTileIdFromPosition(localPos);

	if (localTileId == -1) return nullptr;

	return &data[localTileId];
}

glm::vec3 sQuadrant::LocalPositionToGlobalPosition(glm::ivec3 localPos)
{
	return glm::vec3(posX * 32 + localPos.x - 15, localPos.y, posZ * 32 + localPos.z - 15);
}

glm::vec3 sQuadrant::TileIdToGlobalPosition(int tileId)
{
	if (tileId < 0) return glm::vec3(0); // <- don't let this happen (like ever)

	// Go to local first
	int localY = tileId / (32 * 32) - 15;
	int localX = (tileId % (32 * 32)) / 32;
	int localZ = tileId % 32;

	return LocalPositionToGlobalPosition(glm::vec3(localX, localY, localZ));
}

void cTransitionTrigger::WalkInteract()
{
	std::cout << "Transitioning..." << std::endl;
	Manager::scene.ChangeScene(sceneDescName, entranceNum);
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
	Manager::render.RemoveModel(mapModel);

	for (std::map<int, sInstancedTile>::iterator it = mapInstancedTiles.begin(); it != mapInstancedTiles.end(); it++)
	{
		Manager::render.RemoveModel(it->second.instancedModel);
	}
}

void cMapManager::Startup()
{
	transitionTiles[HEDGE_LEFT].push_back(glm::ivec3(1, 0, 0));
	transitionTiles[HEDGE_LEFT].push_back(glm::ivec3(2, 0, 0));
	transitionTiles[HEDGE_LEFT].push_back(glm::ivec3(3, 0, 0));

	transitionTiles[HEDGE_RIGHT].push_back(glm::ivec3(1, 0, 2));
	transitionTiles[HEDGE_RIGHT].push_back(glm::ivec3(2, 0, 2));
	transitionTiles[HEDGE_RIGHT].push_back(glm::ivec3(3, 0, 2));

	mapModel = Manager::render.CreateRenderModel();
	mapModel->position = glm::vec3(0.5f, 0.f, 0.5f);

	arenaModel = Manager::render.CreateRenderModel(true);

	LoadMap("DemoTownDesc.json", 0);
	//LoadMap("GrassRouteDemoDesc.json", 0);
}

void cMapManager::Shutdown()
{
	Manager::render.RemoveModel(mapModel);
	Manager::render.RemoveModel(arenaModel);

	for (std::map<int, sInstancedTile>::iterator it = mapInstancedTiles.begin(); it != mapInstancedTiles.end(); it++)
	{
		Manager::animation.RemoveAnimation(it->second.instancedModel.get()->animation);
		Manager::render.RemoveModel(it->second.instancedModel);
	}

	for (std::map<int, sInstancedTile>::iterator it = arenaInstancedTiles.begin(); it != arenaInstancedTiles.end(); it++)
	{
		Manager::animation.RemoveAnimation(it->second.instancedModel.get()->animation);
		Manager::render.RemoveModel(it->second.instancedModel);
	}

	delete opponentSpriteModel;
	delete playerSpriteModel;
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

void cMapManager::LoadArena(std::string arenaDescriptionFile)
{
	FILE* fp = 0;
	fopen_s(&fp, (ARENAS_PATH + arenaDescriptionFile).c_str(), "rb"); // non-Windows use "r"

	// OPTIMIZATION: best buffer size might be different
	char readBuffer[4096];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	rapidjson::Document d;
	d.ParseStream(is);

	if (fp == 0) return;
	fclose(fp);

	// Load arena model
	std::string mapModelName = d["arenaModelFileName"].GetString();
	Manager::render.LoadModel(mapModelName, "scene");
	arenaModel->meshName = mapModelName;

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
		arenaInstancedTiles[tileId].instancedModel = Manager::render.CreateAnimatedModel(static_cast<eAnimatedModel>(animationType), true);
		arenaInstancedTiles[tileId].instancedModel->meshName = currInstancedTile["meshName"].GetString();
		arenaInstancedTiles[tileId].instancedModel->orientation.y = glm::radians(meshOrientationY);
		arenaInstancedTiles[tileId].modelOffset = meshPosOffset;
	}

	// Load detail file
	std::string arenaDetailFileName = d["arenaDetailFileName"].GetString();
	std::ifstream pdsmap(ARENAS_PATH + arenaDetailFileName);

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

					if (arenaInstancedTiles.find(tileId) != arenaInstancedTiles.end()) // it exists
					{
						glm::vec4 newOffset = glm::vec4((newQuad.posX * 32 - 15 + x), currHeight, (newQuad.posZ * 32 - 15 + z), 1.f);
						newOffset.x += arenaInstancedTiles[tileId].modelOffset.x;
						newOffset.y += arenaInstancedTiles[tileId].modelOffset.y;
						newOffset.z += arenaInstancedTiles[tileId].modelOffset.z;

						arenaInstancedTiles[tileId].instanceOffsets.push_back(newOffset);
					}
				}
			}
		}

		pdsmap >> currToken; // this should be mapend
		pdsmap >> currToken; // if there is another quad, this will be mapstart
	}
	// end here

	pdsmap.close();

	// Load tile specific animations
	for (std::map<int, sInstancedTile>::iterator it = arenaInstancedTiles.begin(); it != arenaInstancedTiles.end(); it++)
	{
		if (it->second.instanceOffsets.size() != 0)
		{
			it->second.instancedModel->InstanceObject(it->second.instanceOffsets);

			if (it->second.instancedModel->animation)
				Manager::animation.AddAnimation(it->second.instancedModel->animation);

			it->second.instanceOffsets.clear();
		}
	}

	if (!opponentSpriteModel)
		opponentSpriteModel = new cBattleSprite(glm::vec3(3.f, 0.f, 1.f));

	if (!playerSpriteModel)
		playerSpriteModel = new cBattleSprite(glm::vec3(-4.f, 0.f, 1.f));
}

void cMapManager::LoadMap(const std::string mapDescriptionFile, const int entranceNumUsed)
{
	// TEMP: will find a more modular way to load necessary models
	Manager::render.LoadModel("SpriteHolder.obj", "sprite");
	Manager::render.LoadModel("SpriteHolder.obj", "snow");
	Manager::render.LoadModel("SpriteHolder.obj", "particle");
	Manager::render.LoadModel("Water_c2.obj", "wave");
	Manager::render.LoadModel("Water_b2.obj", "wave");
	Manager::render.LoadModel("Water_bl2.obj", "wave");
	Manager::render.LoadModel("sea_water2.obj", "ocean");
	Manager::render.LoadModel("Foam_b2.obj", "foam");
	Manager::render.LoadModel("Foam_bl2.obj", "foam");
	Manager::render.LoadModel("Foam_c2.obj", "foam");
	Manager::render.LoadModel("Foam_c2.obj", "foam");
	Manager::render.LoadModel("r0_treePine.obj", "tree");

	Manager::render.LoadSpriteSheet("Nate.png", 3, 8, false);

	//Manager::render.LoadTexture("SnowFlake1.png");
	//Manager::render.LoadTexture("SnowFlake2.png");
	Manager::render.LoadTexture("SnowFlake3.png");
	Manager::render.LoadTexture("HitParticle.png");

	// Load json info file with rapidjson
	FILE* fp = 0;
	fopen_s(&fp, (MAPS_PATH + mapDescriptionFile).c_str(), "rb"); // non-Windows use "r"

	// OPTIMIZATION: best buffer size might be different
	char readBuffer[4096];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

	rapidjson::Document d;
	d.ParseStream(is);

	if (fp == 0) return;
	fclose(fp);
	
	// Load new map
	std::string mapModelName = d["mapModelFileName"].GetString();
	Manager::render.LoadModel(mapModelName, "scene");
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
		std::vector<glm::ivec3> newTileWalkableOffsets;
		std::vector<glm::ivec3> newTileUnwalkableOffsets;

		rapidjson::Value& walkableData = correctionTileData[i]["walkableTiles"];
		for (unsigned int j = 0; j < walkableData.Size(); j++)
		{
			glm::ivec3 newWalkableOffset;
			newWalkableOffset.x = walkableData[j]["x"].GetInt();
			newWalkableOffset.y = walkableData[j]["y"].GetInt();
			newWalkableOffset.z = walkableData[j]["z"].GetInt();
			newTileWalkableOffsets.push_back(newWalkableOffset);
		}

		rapidjson::Value& unwalkableData = correctionTileData[i]["unwalkableTiles"];
		for (unsigned int j = 0; j < unwalkableData.Size(); j++)
		{
			glm::ivec3 newUnwalkableOffset;
			newUnwalkableOffset.x = unwalkableData[j]["x"].GetInt();
			newUnwalkableOffset.y = unwalkableData[j]["y"].GetInt();
			newUnwalkableOffset.z = unwalkableData[j]["z"].GetInt();
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
		mapInstancedTiles[tileId].instancedModel = Manager::render.CreateAnimatedModel(static_cast<eAnimatedModel>(animationType));
		mapInstancedTiles[tileId].instancedModel->meshName = currInstancedTile["meshName"].GetString();
		mapInstancedTiles[tileId].instancedModel->orientation.y = glm::radians(meshOrientationY);
		mapInstancedTiles[tileId].modelOffset = meshPosOffset;
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
				for (unsigned int j = 0; j < spawnTileId.Size(); j++)
				{
					spawnTileIds.push_back(spawnTileId[j].GetInt());
				}
			}
		}
	}

	// TEMP: loading specific wild encounters for all maps for now
	Manager::scene.LoadSpawnData(406, 0, 0, Pokemon::TALL_GRASS, 0, "");
	Manager::scene.LoadSpawnData(678, 0, 0, Pokemon::TALL_GRASS, 0, "");

	// Load collision map
	std::string collisionMapFileName = d["mapCollisionFileName"].GetString();
	std::ifstream pdsmap(MAPS_PATH + collisionMapFileName);

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

					sTile* currTile = newQuad.GetTileFromLocalPosition(glm::ivec3(x, currHeight, z));

					if (!currTile->isUnchangeable && walkableTiles.find(tileId) != walkableTiles.end()) // is walkable
					{						
						currTile->isWalkable = true;

						// Walkable correction tiles
						for (unsigned int i = 0; i < walkableTiles[tileId].walkableOffsets.size(); i++)
						{
							int correctionX = x + walkableTiles[tileId].walkableOffsets[i].x;
							int correctionZ = z + walkableTiles[tileId].walkableOffsets[i].z;
							int correctionHeight = currHeight + walkableTiles[tileId].walkableOffsets[i].y;

							newQuad.GetTileFromLocalPosition(glm::ivec3(correctionX, correctionHeight, correctionZ))->isWalkable = true;
						}

						// Unwalkable correction tiles
						for (unsigned int i = 0; i < walkableTiles[tileId].unwalkableOffsets.size(); i++)
						{
							int correctionX = x + walkableTiles[tileId].unwalkableOffsets[i].x;
							int correctionZ = z + walkableTiles[tileId].unwalkableOffsets[i].z;
							int correctionHeight = currHeight + walkableTiles[tileId].unwalkableOffsets[i].y;

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

					if (mapInstancedTiles.find(tileId) != mapInstancedTiles.end()) // it exists
					{
						glm::vec4 newOffset = glm::vec4((newQuad.posX * 32 - 15 + x), currHeight, (newQuad.posZ * 32 - 15 + z), 1.f);
						newOffset.x += mapInstancedTiles[tileId].modelOffset.x;
						newOffset.y += mapInstancedTiles[tileId].modelOffset.y;
						newOffset.z += mapInstancedTiles[tileId].modelOffset.z;

						mapInstancedTiles[tileId].instanceOffsets.push_back(newOffset);
					}
				}
			}
		}

		pdsmap >> currToken; // this should be mapend
		pdsmap >> currToken; // if there is another quad, this will be mapstart
	}
	// end here

	pdsmap.close();

	// Load transition tiles
	if (d.HasMember("sceneTransitions"))
	{
		rapidjson::Value& sceneTramsitionsData = d["sceneTransitions"];
		for (unsigned int i = 0; i < sceneTramsitionsData.Size(); i++)
		{
			rapidjson::Value& currTransitionTile = sceneTramsitionsData[i];

			int quadX = currTransitionTile["quadCoord"]["x"].GetInt();
			int quadZ = currTransitionTile["quadCoord"]["z"].GetInt();

			sQuadrant* quad = nullptr;
			for (int q = 0; q < quads.size(); q++)
			{
				if (quads[q].posX == quadX && quads[q].posZ == quadZ)
				{
					quad = &quads[q];
					break;
				}
			}
			if (!quad) continue;

			std::string transitionTo = currTransitionTile["transitionTo"].GetString();
			int entranceNum = currTransitionTile["newSceneEntranceId"].GetInt();

			triggers.emplace_back(transitionTo, entranceNum);
			cTransitionTrigger& transitionTrigger = triggers.back();

			int tileType = currTransitionTile["tileType"].GetInt();
			glm::ivec3 localPos;
			localPos.x = currTransitionTile["localQuadPos"]["x"].GetInt();
			localPos.y = currTransitionTile["localQuadPos"]["y"].GetInt();
			localPos.z = currTransitionTile["localQuadPos"]["z"].GetInt();
			for (int j = 0; j < transitionTiles[static_cast<eTransitionTileTypes>(tileType)].size(); j++)
			{
				glm::ivec3 finalPos = localPos + transitionTiles[(eTransitionTileTypes)tileType][j];

				quad->GetTileFromLocalPosition(finalPos)->entity = &transitionTrigger;
			}
		}

		// Set player position based on entrance used
		if (entranceNumUsed >= 0 && entranceNumUsed < sceneTramsitionsData.Size())
		{
			rapidjson::Value& transitionTileUsed = sceneTramsitionsData[entranceNumUsed];

			int quadX = transitionTileUsed["quadCoord"]["x"].GetInt();
			int quadZ = transitionTileUsed["quadCoord"]["z"].GetInt();

			sQuadrant* entranceQuad = nullptr;
			for (int q = 0; q < quads.size(); q++)
			{
				if (quads[q].posX == quadX && quads[q].posZ == quadZ)
				{
					entranceQuad = &quads[q];
					break;
				}
			}

			if (entranceQuad)
			{
				glm::ivec3 spawnOffset;
				spawnOffset.x = transitionTileUsed["playerSpawnOffset"]["x"].GetInt();
				spawnOffset.y = transitionTileUsed["playerSpawnOffset"]["y"].GetInt();
				spawnOffset.z = transitionTileUsed["playerSpawnOffset"]["z"].GetInt();

				glm::ivec3 localPos;
				localPos.x = transitionTileUsed["localQuadPos"]["x"].GetInt();
				localPos.y = transitionTileUsed["localQuadPos"]["y"].GetInt();
				localPos.z = transitionTileUsed["localQuadPos"]["z"].GetInt();

				glm::ivec3 finalPlayerLocalPos = localPos + spawnOffset;
				glm::ivec3 finalPlayerPos = entranceQuad->LocalPositionToGlobalPosition(finalPlayerLocalPos);
				Player::playerChar->spriteModel->model.get()->position = finalPlayerPos;
				Player::playerChar->position = finalPlayerPos;
				entranceQuad->GetTileFromLocalPosition(finalPlayerLocalPos)->entity = Player::playerChar;

				glm::ivec3 partnerOffest = glm::ivec3(0);
				switch (transitionTileUsed["tileType"].GetInt())
				{
				case eTransitionTileTypes::HEDGE_LEFT:
					partnerOffest.z = 1;
					//Player::playerPartner.get()->spriteModel->AnimateMovement(LEFT, false, FAILURE); // investigate why this teleports partner to origin
					break;
				case eTransitionTileTypes::HEDGE_RIGHT:
					partnerOffest.z = -1;
					//Player::playerPartner.get()->spriteModel->AnimateMovement(RIGHT, false, FAILURE);
					break;
				default:
					break;
				}

				glm::ivec3 partnerLocalPos = finalPlayerLocalPos + partnerOffest;
				glm::ivec3 partnerPos = entranceQuad->LocalPositionToGlobalPosition(partnerLocalPos);
				Player::playerPartner.get()->spriteModel->model.get()->position = partnerPos;
				Player::playerPartner.get()->position = partnerPos;
				//entranceQuad->GetTileFromLocalPosition(partnerLocalPos)->entity = Player::playerPartner.get();
			}
		}
	}

	// Load tile specific animations
	for (std::map<int, sInstancedTile>::iterator it = mapInstancedTiles.begin(); it != mapInstancedTiles.end(); it++)
	{
		if (it->second.instanceOffsets.size() != 0)
		{
			it->second.instancedModel->InstanceObject(it->second.instanceOffsets);

			if (it->second.instancedModel->animation)
				Manager::animation.AddAnimation(it->second.instancedModel->animation);

			it->second.instanceOffsets.clear();
		}
	}

	std::string arenaDescFileName = d["arenaDescFileName"].GetString();
	LoadArena(arenaDescFileName);
}

void cMapManager::UnloadMap()
{
	for (std::map<int, sInstancedTile>::iterator it = mapInstancedTiles.begin(); it != mapInstancedTiles.end(); it++)
	{
		Manager::animation.RemoveAnimation(it->second.instancedModel.get()->animation);
		Manager::render.RemoveModel(it->second.instancedModel);
	}
	mapInstancedTiles.clear();

	for (std::map<int, sInstancedTile>::iterator it = arenaInstancedTiles.begin(); it != arenaInstancedTiles.end(); it++)
	{
		Manager::animation.RemoveAnimation(it->second.instancedModel.get()->animation);
		Manager::render.RemoveModel(it->second.instancedModel);
	}
	arenaInstancedTiles.clear();

	quads.clear();
	walkableTiles.clear();
	triggers.clear();
}

//void cMapManager::ChangeScene(const std::string newSceneDescFile)
//{
//	UnloadMap();
//	LoadMap(newSceneDescFile);
//}

sTile* cMapManager::GetTile(glm::ivec3 worldPosition)
{
	if (sQuadrant* quad = GetQuad(worldPosition.x, worldPosition.z))
	{
		// TODO: probably a good idea to make a world to local function
		glm::ivec3 localPosition = worldPosition;
		localPosition.x += 15 - 32 * quad->posX;
		localPosition.z += 15 - 32 * quad->posZ;

		return quad->GetTileFromLocalPosition(localPosition);
	}

	return nullptr;
}

sTile* cMapManager::GetRandomSpawnTile(glm::vec3& globalPositionOut)
{
	// Semi random: make sure to pick a tile close to player
	glm::vec3 playerPos = Player::GetPlayerPosition();

	sQuadrant* spawnQuad = nullptr;
	int findQuadAttempts = 0;
	bool foundValidQuad = false;
	while (!foundValidQuad)
	{
		// Pick a random adjacent quad (assuming player quad is valid)
		int randQuadOffsetX = (rand() % 3) - 1; // [-1,1]
		int randQuadOffsetZ = (rand() % 3) - 1; // [-1,1]

		spawnQuad = GetQuad((int)playerPos.x + (randQuadOffsetX * 32), (int)playerPos.z + (randQuadOffsetZ * 32));
		if (spawnQuad && spawnQuad->localSpawnTiles.size() != 0)
			foundValidQuad = true;

		findQuadAttempts++;
		if (findQuadAttempts >= 5) 
			return nullptr;
	}

	glm::vec3 tilePos;
	sTile* spawnTile = spawnQuad->GetRandomSpawnTile(tilePos);

	if (spawnTile)
		spawnQuad->wildPokemonCount++;

	globalPositionOut = tilePos;
	return spawnTile;
}

void cMapManager::RemoveEntityFromTile(glm::ivec3 worldPosition)
{
	sTile* tile = GetTile(worldPosition);

	if (!tile) return;

	tile->entity = nullptr;
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
