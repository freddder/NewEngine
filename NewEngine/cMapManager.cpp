#include "cMapManager.h"
#include "Global.h"

#include <sstream>
#include <fstream>
#include <iostream>

cMapManager::cMapManager()
{
	mapModel = new cModel();
	mapModel->position = glm::vec3(0.5f, 0.f, 0.5f);
	mapModel->meshName = "TestMap1.obj";
	g_set_Models.insert(mapModel);

	walkableTiles.insert(100);
}

cMapManager::~cMapManager()
{
	delete mapModel;
}

void cMapManager::LoadMap(std::string mapModelName, std::string mapDescName)
{
	//g_set_Models.erase(mapModel);

    std::ifstream file("assets/models/TestMap1.pdsmap");

    if (!file.is_open())
        return;

	std::string currToken;

	sQuadrant newQuad;
	int tempLayers[32][32][8];

	while (file >> currToken)
	{
		if (currToken == "mapstart")
		{
			file >> newQuad.quadX;
			file >> newQuad.quadZ;

			break;
		}
	}

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
				file >> tempLayers[x][z][layerId];

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

				currHeight += 15;

				if (walkableTiles.find(tempLayers[x][z][currHeight]) != walkableTiles.end()) // contains
				{
					if(!newQuad.quadData[x][z][currHeight].isUnchangeable)
						newQuad.quadData[x][z][currHeight].isWalkable = true;
				}
				else if (tempLayers[x][z][currHeight] != -1) // desn't contain
				{
					newQuad.quadData[x][z][currHeight].isWalkable = false;
					newQuad.quadData[x][z][currHeight].isUnchangeable = true;
				}
				//std::cout << currToken << " ";
			}

			//std::cout << std::endl;
		}

		//std::cout << std::endl;
	}

	file.close();
}

int cMapManager::MoveEntity(glm::vec3 currPosition, int direction)
{
	glm::vec3 desiredLocation = currPosition;

	// direction = 0 -> UP
	// direction = 1 -> DOWN
	// direction = 2 -> LEFT
	// direction = 3 -> RIGHT
	if(direction == 0)
		desiredLocation.
}