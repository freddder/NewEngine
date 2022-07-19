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

	while (file >> currToken)
	{
		//std::cout << currToken << std::endl;
	}

	file.close();
}
