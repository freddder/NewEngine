#include "cMapManager.h"
#include "Global.h"

#include <sstream>
#include <fstream>
#include <iostream>

cMapManager::cMapManager()
{
	mapModel = new cModel();
	mapModel->position = glm::vec3(0.5f, 0.f, 0.5f);
	mapModel->meshName = "TestMap2.obj";
	g_set_Models.insert(mapModel);

	walkableTiles[100]; 
	walkableTiles[101];
	walkableTiles[103];
	walkableTiles[52];
	walkableTiles[53];
	walkableTiles[54];
	walkableTiles[55];
	walkableTiles[56];
	walkableTiles[57];
	walkableTiles[58];
	walkableTiles[59];
	walkableTiles[92];
	walkableTiles[95];

	//	^ x
	//  > z
	walkableTiles[135].walkableOffsets.push_back(glm::vec3(0.f, 1.f, 1.f));
	walkableTiles[135].walkableOffsets.push_back(glm::vec3(1.f, 2.f, 1.f));
	walkableTiles[135].walkableOffsets.push_back(glm::vec3(2.f, 3.f, 1.f));
	walkableTiles[135].walkableOffsets.push_back(glm::vec3(3.f, 4.f, 1.f));
	walkableTiles[135].walkableOffsets.push_back(glm::vec3(4.f, 5.f, 1.f));
	walkableTiles[135].unwalkableOffsets.push_back(glm::vec3(0.f, 0.f, 0.f));
	walkableTiles[135].unwalkableOffsets.push_back(glm::vec3(1.f, 0.f, 0.f));
	walkableTiles[135].unwalkableOffsets.push_back(glm::vec3(0.f, 0.f, 1.f));
	walkableTiles[135].unwalkableOffsets.push_back(glm::vec3(1.f, 0.f, 0.f));
	walkableTiles[135].unwalkableOffsets.push_back(glm::vec3(2.f, 0.f, 0.f));
	walkableTiles[135].unwalkableOffsets.push_back(glm::vec3(3.f, 0.f, 0.f));

	walkableTiles[136].walkableOffsets.push_back(glm::vec3(0.f, 1.f, 0.f));
	walkableTiles[136].walkableOffsets.push_back(glm::vec3(1.f, 2.f, 0.f));
	walkableTiles[136].walkableOffsets.push_back(glm::vec3(2.f, 3.f, 0.f));
	walkableTiles[136].walkableOffsets.push_back(glm::vec3(3.f, 4.f, 0.f));
	walkableTiles[136].walkableOffsets.push_back(glm::vec3(4.f, 5.f, 0.f));
	walkableTiles[136].unwalkableOffsets.push_back(glm::vec3(0.f, 0.f, 0.f));

	walkableTiles[137].walkableOffsets.push_back(glm::vec3(0.f, 1.f, 0.f));
	walkableTiles[137].walkableOffsets.push_back(glm::vec3(1.f, 2.f, 0.f));
	walkableTiles[137].walkableOffsets.push_back(glm::vec3(2.f, 3.f, 0.f));
	walkableTiles[137].walkableOffsets.push_back(glm::vec3(3.f, 4.f, 0.f));
	walkableTiles[137].walkableOffsets.push_back(glm::vec3(4.f, 5.f, 0.f));
	walkableTiles[137].unwalkableOffsets.push_back(glm::vec3(0.f, 0.f, 0.f));
	walkableTiles[137].unwalkableOffsets.push_back(glm::vec3(0.f, 0.f, 1.f));
	walkableTiles[137].unwalkableOffsets.push_back(glm::vec3(1.f, 0.f, 1.f));
	walkableTiles[137].unwalkableOffsets.push_back(glm::vec3(2.f, 0.f, 1.f));
	walkableTiles[137].unwalkableOffsets.push_back(glm::vec3(3.f, 0.f, 1.f));
	walkableTiles[137].unwalkableOffsets.push_back(glm::vec3(4.f, 0.f, 1.f));

	walkableTiles[171].walkableOffsets.push_back(glm::vec3(1.f, 0.f, 0.f));
	walkableTiles[171].walkableOffsets.push_back(glm::vec3(2.f, 0.f, 0.f));
}

cMapManager::~cMapManager()
{
	delete mapModel;
}

void cMapManager::LoadMap(std::string mapModelName, std::string mapDescName)
{
	//g_set_Models.erase(mapModel);

    std::ifstream file("assets/models/TestMap2.pdsmap");

    if (!file.is_open())
        return;

	std::string currToken;

	// make sure reader is at first mapstart
	while (file >> currToken)
	{
		if (currToken == "mapstart")
			break;
	}

	// start here
	while (currToken == "mapstart")
	{
		sQuadrant newQuad;
		int tempLayers[32][32][8];

		//while (file >> currToken)
		//{
		//	if (currToken == "mapstart")
		//	{
		//		file >> newQuad.quadX;
		//		file >> newQuad.quadZ;

		//		break;
		//	}
		//}

		// set quadrant coords
		file >> newQuad.quadX;
		file >> newQuad.quadZ;

		// skip areaindex
		file >> currToken;
		file >> currToken;

		for (int layerId = 0; layerId < 8; layerId++)
		{
			file >> currToken;

			if (currToken != "tilegrid")
				break;

			for (int x = 0; x < 32; x++)
			{
				for (int z = 0; z < 32; z++)
				{
					int currTile;
					file >> currTile;
					tempLayers[x][z][layerId] = currTile;
					
					//file >> tempLayers[x][z][layerId];

					//file >> currToken;
					//std::cout << currToken << " ";
				}

				//std::cout << std::endl;
			}

			//std::cout << std::endl;
		}

		for (int layerId = 0; layerId < 8; layerId++)
		{
			file >> currToken;

			if (currToken != "heightgrid")
				break;

			for (int x = 0; x < 32; x++)
			{
				for (int z = 0; z < 32; z++)
				{
					int currHeight;
					file >> currHeight;

					//currHeight += 15;

					int currTile = tempLayers[x][z][layerId];

					if (walkableTiles.find(currTile) != walkableTiles.end()) // contains
					{
						if (!newQuad.quadData[x][z][currHeight].isUnchangeable)
						{
							newQuad.quadData[x][z][currHeight].isWalkable = true;
							//newQuad.quadMapData[x][z][currHeight].isWalkable = true;

							for (unsigned int i = 0; i < walkableTiles[currTile].walkableOffsets.size(); i++)
							{
								//int newX = x + (int)walkableTiles[currTile].walkableOffsets[i].x;
								//int newZ = z + (int)walkableTiles[currTile].walkableOffsets[i].z;
								//int newY = currHeight + (int)walkableTiles[currTile].walkableOffsets[i].y;

								newQuad.quadData[x + (int)walkableTiles[currTile].walkableOffsets[i].x]
												[z + (int)walkableTiles[currTile].walkableOffsets[i].z]
												[currHeight + (int)walkableTiles[currTile].walkableOffsets[i].y].isWalkable = true;

								//newQuad.quadMapData[x + (int)walkableTiles[currTile].walkableOffsets[i].x]
								//				   [z + (int)walkableTiles[currTile].walkableOffsets[i].z]
								//				   [currHeight + (int)walkableTiles[currTile].walkableOffsets[i].y].isWalkable = true;
							}

							for (unsigned int i = 0; i < walkableTiles[currTile].unwalkableOffsets.size(); i++)
							{
								newQuad.quadData[(x + (int)walkableTiles[currTile].unwalkableOffsets[i].x)]
												[(z + (int)walkableTiles[currTile].unwalkableOffsets[i].z)]
												[(currHeight + (int)walkableTiles[currTile].unwalkableOffsets[i].y)].isWalkable = false;

								newQuad.quadData[(x + (int)walkableTiles[currTile].unwalkableOffsets[i].x)]
												[(z + (int)walkableTiles[currTile].unwalkableOffsets[i].z)]
												[(currHeight + (int)walkableTiles[currTile].unwalkableOffsets[i].y)].isUnchangeable = true;

								//newQuad.quadMapData[(x + (int)walkableTiles[currTile].unwalkableOffsets[i].x)]
								//				   [(z + (int)walkableTiles[currTile].unwalkableOffsets[i].z)]
								//				   [(currHeight + (int)walkableTiles[currTile].unwalkableOffsets[i].y)].isWalkable = false;

								//newQuad.quadMapData[(x + (int)walkableTiles[currTile].unwalkableOffsets[i].x)]
								//				   [(z + (int)walkableTiles[currTile].unwalkableOffsets[i].z)]
								//				   [(currHeight + (int)walkableTiles[currTile].unwalkableOffsets[i].y)].isUnchangeable = true;
							}
						}
					}
					else if (tempLayers[x][z][layerId] != -1) // desn't contain
					{
						newQuad.quadData[x][z][currHeight].isWalkable = false;
						newQuad.quadData[x][z][currHeight].isUnchangeable = true;

						//newQuad.quadMapData[x][z][currHeight].isWalkable = false;
						//newQuad.quadMapData[x][z][currHeight].isUnchangeable = true;
					}
					//std::cout << currToken << " ";
				}

				//std::cout << std::endl;
			}

			//std::cout << std::endl;
		}

		quads.push_back(newQuad);

		file >> currToken; // this should be mapend
		file >> currToken; // if there is another map, this will be mapstart
	}
	// end here

	file.close();
}

int cMapManager::MoveEntity(glm::vec3 currPosition, int direction)
{
	glm::vec3 desiredLocation = currPosition;

	// direction = 0 -> UP
	// direction = 1 -> DOWN
	// direction = 2 -> LEFT
	// direction = 3 -> RIGHT
	if (direction == 0)
		desiredLocation.x += 1.f;
	else if (direction == 1)
		desiredLocation.x -= 1.f;
	else if (direction == 2)
		desiredLocation.z -= 1.f;
	else if (direction == 3)
		desiredLocation.z += 1.f;

	if (desiredLocation.x + 15 < 0 || desiredLocation.z + 15 < 0)
		return 0;

	desiredLocation.x += 15;
	//desiredLocation.y += 15;
	desiredLocation.z += 15;

	int quadXCoord = ((int)desiredLocation.z) / 32;
	int quadZCoord = ((int)desiredLocation.x) / 32;

	for (std::vector<sQuadrant>::iterator it = quads.begin(); it != quads.end(); it++)
	{
		if (quadXCoord == it->quadX && quadZCoord == it->quadZ)
		{
			desiredLocation.z -= 32 * quadXCoord;
			desiredLocation.x -= 32 * quadZCoord;

			if (it->quadData[(int)desiredLocation.x][(int)desiredLocation.z][(int)desiredLocation.y].isWalkable)
				return 1; // same height
			else if (it->quadData[(int)desiredLocation.x][(int)desiredLocation.z][(int)desiredLocation.y + 1].isWalkable)
				return 2; // go up
			else if (it->quadData[(int)desiredLocation.x][(int)desiredLocation.z][(int)desiredLocation.y - 1].isWalkable)
				return 3; // go down

			return 0;
		}
	}

	return 0;
}