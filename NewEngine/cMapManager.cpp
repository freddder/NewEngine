#include "cMapManager.h"

#include <sstream>
#include <fstream>
#include "cFloatAnimation.h"
#include "cRenderManager.h"
#include "cAnimationManager.h"

cMapManager* cMapManager::singleton = NULL;

cMapManager::cMapManager()
{
	mapModel = new cRenderModel();
	mapModel->position = glm::vec3(0.5f, 0.f, 0.5f);
	mapModel->meshName = "WinterTest.obj";
	cRenderManager::GetInstance()->AddModel(mapModel);

	// wave
	instancedTiles[118].instancedModel = new cWaveModel();
	instancedTiles[118].instancedModel->meshName = "Water_b2.obj";
	instancedTiles[118].modelOffset = glm::vec3(-0.5f, 0.f, -0.5f);
	instancedTiles[118].animation = new cSinAnimation(static_cast<cWaveModel*>(instancedTiles[118].instancedModel)->textureOffset, 2, 0);
	static_cast<cSinAnimation*>(instancedTiles[118].animation)->AddKeyFrame(sKeyFrameVec3(7.f, glm::vec3(360.f, 0.f, 0.f)));
	instancedTiles[118].animation->isRepeat = true;

	instancedTiles[120].instancedModel = new cWaveModel();
	instancedTiles[120].instancedModel->meshName = "Water_b2.obj";
	instancedTiles[120].instancedModel->orientation.y = glm::radians(90.f);
	instancedTiles[120].modelOffset = glm::vec3(-0.5f, 0.f, 2.5f);
	instancedTiles[120].animation = new cSinAnimation(static_cast<cWaveModel*>(instancedTiles[120].instancedModel)->textureOffset, 2, 0);
	static_cast<cSinAnimation*>(instancedTiles[120].animation)->AddKeyFrame(sKeyFrameVec3(7.f, glm::vec3(360.f, 0.f, 0.f)));
	instancedTiles[120].animation->isRepeat = true;

	instancedTiles[122].instancedModel = new cWaveModel();
	instancedTiles[122].instancedModel->meshName = "Water_b2.obj";
	instancedTiles[122].instancedModel->orientation.y = glm::radians(-90.f);
	instancedTiles[122].modelOffset = glm::vec3(0.5f, 0.f, -0.5f);
	instancedTiles[122].animation = new cSinAnimation(static_cast<cWaveModel*>(instancedTiles[122].instancedModel)->textureOffset, 2, 0);
	static_cast<cSinAnimation*>(instancedTiles[122].animation)->AddKeyFrame(sKeyFrameVec3(7.f, glm::vec3(360.f, 0.f, 0.f)));
	instancedTiles[122].animation->isRepeat = true;

	instancedTiles[124].instancedModel = new cWaveModel();
	instancedTiles[124].instancedModel->meshName = "Water_b2.obj";
	instancedTiles[124].instancedModel->orientation.y = glm::radians(180.f);
	instancedTiles[124].modelOffset = glm::vec3(2.5f, 0.f, 0.5f);
	instancedTiles[124].animation = new cSinAnimation(static_cast<cWaveModel*>(instancedTiles[124].instancedModel)->textureOffset, 2, 0);
	static_cast<cSinAnimation*>(instancedTiles[124].animation)->AddKeyFrame(sKeyFrameVec3(7.f, glm::vec3(360.f, 0.f, 0.f)));
	instancedTiles[124].animation->isRepeat = true;
	
	instancedTiles[119].instancedModel = new cWaveModel();
	instancedTiles[119].instancedModel->meshName = "Water_c2.obj";
	instancedTiles[119].modelOffset = glm::vec3(-0.5f, 0.f, -0.5f);
	instancedTiles[119].animation = new cSinAnimation(static_cast<cWaveModel*>(instancedTiles[119].instancedModel)->textureOffset, 2, 0);
	static_cast<cSinAnimation*>(instancedTiles[119].animation)->AddKeyFrame(sKeyFrameVec3(7.f, glm::vec3(360.f, 0.f, 0.f)));
	instancedTiles[119].animation->isRepeat = true;

	instancedTiles[117].instancedModel = new cWaveModel();
	instancedTiles[117].instancedModel->meshName = "Water_c2.obj";
	instancedTiles[117].instancedModel->orientation.y = glm::radians(90.f);
	instancedTiles[117].modelOffset = glm::vec3(-0.5f, 0.f, 2.5f);
	instancedTiles[117].animation = new cSinAnimation(static_cast<cWaveModel*>(instancedTiles[117].instancedModel)->textureOffset, 2, 0);
	static_cast<cSinAnimation*>(instancedTiles[117].animation)->AddKeyFrame(sKeyFrameVec3(7.f, glm::vec3(360.f, 0.f, 0.f)));
	instancedTiles[117].animation->isRepeat = true;

	instancedTiles[123].instancedModel = new cWaveModel();
	instancedTiles[123].instancedModel->meshName = "Water_c2.obj";
	instancedTiles[123].instancedModel->orientation.y = glm::radians(180.f);
	instancedTiles[123].modelOffset = glm::vec3(2.5f, 0.f, 2.5f);
	instancedTiles[123].animation = new cSinAnimation(static_cast<cWaveModel*>(instancedTiles[123].instancedModel)->textureOffset, 2, 0);
	static_cast<cSinAnimation*>(instancedTiles[123].animation)->AddKeyFrame(sKeyFrameVec3(7.f, glm::vec3(360.f, 0.f, 0.f)));
	instancedTiles[123].animation->isRepeat = true;

	instancedTiles[125].instancedModel = new cWaveModel();
	instancedTiles[125].instancedModel->meshName = "Water_c2.obj";
	instancedTiles[125].instancedModel->orientation.y = glm::radians(270.f);
	instancedTiles[125].modelOffset = glm::vec3(2.5f, 0.f, -0.5f);
	instancedTiles[125].animation = new cSinAnimation(static_cast<cWaveModel*>(instancedTiles[125].instancedModel)->textureOffset, 2, 0);
	static_cast<cSinAnimation*>(instancedTiles[125].animation)->AddKeyFrame(sKeyFrameVec3(7.f, glm::vec3(360.f, 0.f, 0.f)));
	instancedTiles[125].animation->isRepeat = true;

	instancedTiles[138].instancedModel = new cWaveModel();
	instancedTiles[138].instancedModel->meshName = "Water_bl2.obj";
	instancedTiles[138].modelOffset = glm::vec3(-0.5f, 0.f, -0.5f);
	instancedTiles[138].animation = new cSinAnimation(static_cast<cWaveModel*>(instancedTiles[138].instancedModel)->textureOffset, 2, 0);
	static_cast<cSinAnimation*>(instancedTiles[138].animation)->AddKeyFrame(sKeyFrameVec3(7.f, glm::vec3(360.f, 0.f, 0.f)));
	instancedTiles[138].animation->isRepeat = true;

	instancedTiles[136].instancedModel = new cWaveModel();
	instancedTiles[136].instancedModel->meshName = "Water_bl2.obj";
	instancedTiles[136].instancedModel->orientation.y = glm::radians(270.f);
	instancedTiles[136].modelOffset = glm::vec3(2.5f, 0.f, -0.5f);
	instancedTiles[136].animation = new cSinAnimation(static_cast<cWaveModel*>(instancedTiles[136].instancedModel)->textureOffset, 2, 0);
	static_cast<cSinAnimation*>(instancedTiles[136].animation)->AddKeyFrame(sKeyFrameVec3(7.f, glm::vec3(360.f, 0.f, 0.f)));
	instancedTiles[136].animation->isRepeat = true;

	instancedTiles[137].instancedModel = new cWaveModel();
	instancedTiles[137].instancedModel->meshName = "Water_bl2.obj";
	instancedTiles[137].instancedModel->orientation.y = glm::radians(180.f);
	instancedTiles[137].modelOffset = glm::vec3(2.5f, 0.f, 2.5f);
	instancedTiles[137].animation = new cSinAnimation(static_cast<cWaveModel*>(instancedTiles[137].instancedModel)->textureOffset, 2, 0);
	static_cast<cSinAnimation*>(instancedTiles[137].animation)->AddKeyFrame(sKeyFrameVec3(7.f, glm::vec3(360.f, 0.f, 0.f)));
	instancedTiles[137].animation->isRepeat = true;

	instancedTiles[139].instancedModel = new cWaveModel();
	instancedTiles[139].instancedModel->meshName = "Water_bl2.obj";
	instancedTiles[139].instancedModel->orientation.y = glm::radians(90.f);
	instancedTiles[139].modelOffset = glm::vec3(-0.5f, 0.f, 2.5f);
	instancedTiles[139].animation = new cSinAnimation(static_cast<cWaveModel*>(instancedTiles[139].instancedModel)->textureOffset, 2, 0);
	static_cast<cSinAnimation*>(instancedTiles[139].animation)->AddKeyFrame(sKeyFrameVec3(7.f, glm::vec3(360.f, 0.f, 0.f)));
	instancedTiles[139].animation->isRepeat = true;

	instancedTiles[130].instancedModel = new cOceanModel();
	instancedTiles[130].instancedModel->meshName = "sea_water2.obj";
	static_cast<cOceanModel*>(instancedTiles[130].instancedModel)->globalUVRatios = glm::vec2(0.35f, 0.35f);
	instancedTiles[130].modelOffset = glm::vec3(-0.5f, 0.f, -0.5f);
	instancedTiles[130].animation = new cSinAnimation(static_cast<cOceanModel*>(instancedTiles[130].instancedModel)->textureOffset, 0.5f, 0);
	static_cast<cSinAnimation*>(instancedTiles[130].animation)->AddKeyFrame(sKeyFrameVec3(6.f, glm::vec3(360.f, 180.f, 0.f)));
	static_cast<cSinAnimation*>(instancedTiles[130].animation)->AddKeyFrame(sKeyFrameVec3(12.f, glm::vec3(720.f, 360.f, 0.f)));
	instancedTiles[130].animation->isRepeat = true;

	// foam
	instancedTiles[144].instancedModel = new cFoamModel();
	instancedTiles[144].instancedModel->meshName = "Foam_b2.obj";
	instancedTiles[144].instancedModel->orientation.y = glm::radians(180.f);
	instancedTiles[144].modelOffset = glm::vec3(0.5f, 0.f, 0.5f);
	instancedTiles[144].animation = new cSinAnimation(static_cast<cWaveModel*>(instancedTiles[144].instancedModel)->textureOffset, 2, 0);
	static_cast<cSinAnimation*>(instancedTiles[144].animation)->AddKeyFrame(sKeyFrameVec3(7.f, glm::vec3(360.f, 0.f, 0.f)));
	instancedTiles[144].animation->isRepeat = true;

	instancedTiles[145].instancedModel = new cFoamModel();
	instancedTiles[145].instancedModel->meshName = "Foam_b2.obj";
	instancedTiles[145].instancedModel->orientation.y = glm::radians(-90.f);
	instancedTiles[145].modelOffset = glm::vec3(0.5f, 0.f, -0.5f);
	instancedTiles[145].animation = new cSinAnimation(static_cast<cWaveModel*>(instancedTiles[145].instancedModel)->textureOffset, 2, 0);
	static_cast<cSinAnimation*>(instancedTiles[145].animation)->AddKeyFrame(sKeyFrameVec3(7.f, glm::vec3(360.f, 0.f, 0.f)));
	instancedTiles[145].animation->isRepeat = true;

	instancedTiles[146].instancedModel = new cFoamModel();
	instancedTiles[146].instancedModel->meshName = "Foam_b2.obj";
	instancedTiles[146].modelOffset = glm::vec3(-0.5f, 0.f, -0.5f);
	instancedTiles[146].animation = new cSinAnimation(static_cast<cWaveModel*>(instancedTiles[146].instancedModel)->textureOffset, 2, 0);
	static_cast<cSinAnimation*>(instancedTiles[146].animation)->AddKeyFrame(sKeyFrameVec3(7.f, glm::vec3(360.f, 0.f, 0.f)));
	instancedTiles[146].animation->isRepeat = true;

	instancedTiles[147].instancedModel = new cFoamModel();
	instancedTiles[147].instancedModel->meshName = "Foam_b2.obj";
	instancedTiles[147].instancedModel->orientation.y = glm::radians(90.f);
	instancedTiles[147].modelOffset = glm::vec3(-0.5f, 0.f, 0.5f);
	instancedTiles[147].animation = new cSinAnimation(static_cast<cWaveModel*>(instancedTiles[147].instancedModel)->textureOffset, 2, 0);
	static_cast<cSinAnimation*>(instancedTiles[147].animation)->AddKeyFrame(sKeyFrameVec3(7.f, glm::vec3(360.f, 0.f, 0.f)));
	instancedTiles[147].animation->isRepeat = true;

	instancedTiles[148].instancedModel = new cFoamModel();
	instancedTiles[148].instancedModel->meshName = "Foam_bl2.obj";
	instancedTiles[148].instancedModel->orientation.y = glm::radians(-90.f);
	instancedTiles[148].modelOffset = glm::vec3(-0.5f, 0.f, -0.5f);
	instancedTiles[148].animation = new cSinAnimation(static_cast<cWaveModel*>(instancedTiles[148].instancedModel)->textureOffset, 2, 0);
	static_cast<cSinAnimation*>(instancedTiles[148].animation)->AddKeyFrame(sKeyFrameVec3(7.f, glm::vec3(360.f, 0.f, 0.f)));
	instancedTiles[148].animation->isRepeat = true;

	instancedTiles[149].instancedModel = new cFoamModel();
	instancedTiles[149].instancedModel->meshName = "Foam_bl2.obj";
	instancedTiles[149].instancedModel->orientation.y = glm::radians(180.f);
	instancedTiles[149].modelOffset = glm::vec3(0.5f, 0.f, -0.5f);
	instancedTiles[149].animation = new cSinAnimation(static_cast<cWaveModel*>(instancedTiles[149].instancedModel)->textureOffset, 2, 0);
	static_cast<cSinAnimation*>(instancedTiles[149].animation)->AddKeyFrame(sKeyFrameVec3(7.f, glm::vec3(360.f, 0.f, 0.f)));
	instancedTiles[149].animation->isRepeat = true;

	instancedTiles[150].instancedModel = new cFoamModel();
	instancedTiles[150].instancedModel->meshName = "Foam_bl2.obj";
	//instancedTiles[150].instancedModel->orientation.y = glm::radians(180.f);
	instancedTiles[150].modelOffset = glm::vec3(-0.5f, 0.f, 0.5f);
	instancedTiles[150].animation = new cSinAnimation(static_cast<cWaveModel*>(instancedTiles[150].instancedModel)->textureOffset, 2, 0);
	static_cast<cSinAnimation*>(instancedTiles[150].animation)->AddKeyFrame(sKeyFrameVec3(7.f, glm::vec3(360.f, 0.f, 0.f)));
	instancedTiles[150].animation->isRepeat = true;

	instancedTiles[151].instancedModel = new cFoamModel();
	instancedTiles[151].instancedModel->meshName = "Foam_bl2.obj";
	instancedTiles[151].instancedModel->orientation.y = glm::radians(90.f);
	instancedTiles[151].modelOffset = glm::vec3(0.5f, 0.f, 0.5f);
	instancedTiles[151].animation = new cSinAnimation(static_cast<cWaveModel*>(instancedTiles[151].instancedModel)->textureOffset, 2, 0);
	static_cast<cSinAnimation*>(instancedTiles[151].animation)->AddKeyFrame(sKeyFrameVec3(7.f, glm::vec3(360.f, 0.f, 0.f)));
	instancedTiles[151].animation->isRepeat = true;

	instancedTiles[152].instancedModel = new cFoamModel();
	instancedTiles[152].instancedModel->meshName = "Foam_c2.obj";
	instancedTiles[152].instancedModel->orientation.y = glm::radians(180.f);
	instancedTiles[152].modelOffset = glm::vec3(0.5f, 0.f, -0.5f);
	instancedTiles[152].animation = new cSinAnimation(static_cast<cWaveModel*>(instancedTiles[152].instancedModel)->textureOffset, 2, 0);
	static_cast<cSinAnimation*>(instancedTiles[152].animation)->AddKeyFrame(sKeyFrameVec3(7.f, glm::vec3(360.f, 0.f, 0.f)));
	instancedTiles[152].animation->isRepeat = true;

	instancedTiles[153].instancedModel = new cFoamModel();
	instancedTiles[153].instancedModel->meshName = "Foam_c2.obj";
	instancedTiles[153].instancedModel->orientation.y = glm::radians(-90.f);
	instancedTiles[153].modelOffset = glm::vec3(-0.5f, 0.f, -0.5f);
	instancedTiles[153].animation = new cSinAnimation(static_cast<cWaveModel*>(instancedTiles[153].instancedModel)->textureOffset, 2, 0);
	static_cast<cSinAnimation*>(instancedTiles[153].animation)->AddKeyFrame(sKeyFrameVec3(7.f, glm::vec3(360.f, 0.f, 0.f)));
	instancedTiles[153].animation->isRepeat = true;

	instancedTiles[154].instancedModel = new cFoamModel();
	instancedTiles[154].instancedModel->meshName = "Foam_c2.obj";
	instancedTiles[154].instancedModel->orientation.y = glm::radians(90.f);
	instancedTiles[154].modelOffset = glm::vec3(0.5f, 0.f, 0.5f);
	instancedTiles[154].animation = new cSinAnimation(static_cast<cWaveModel*>(instancedTiles[154].instancedModel)->textureOffset, 2, 0);
	static_cast<cSinAnimation*>(instancedTiles[154].animation)->AddKeyFrame(sKeyFrameVec3(7.f, glm::vec3(360.f, 0.f, 0.f)));
	instancedTiles[154].animation->isRepeat = true;

	instancedTiles[155].instancedModel = new cFoamModel();
	instancedTiles[155].instancedModel->meshName = "Foam_c2.obj";
	//instancedTiles[155].instancedModel->orientation.y = glm::radians(90.f);
	instancedTiles[155].modelOffset = glm::vec3(-0.5f, 0.f, 0.5f);
	instancedTiles[155].animation = new cSinAnimation(static_cast<cWaveModel*>(instancedTiles[155].instancedModel)->textureOffset, 2, 0);
	static_cast<cSinAnimation*>(instancedTiles[155].animation)->AddKeyFrame(sKeyFrameVec3(7.f, glm::vec3(360.f, 0.f, 0.f)));
	instancedTiles[155].animation->isRepeat = true;

	// grass
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

	// snow
	walkableTiles[191];

	//	^ x
	//  > z

	// left stair5
	walkableTiles[148].walkableOffsets.push_back(glm::vec3(0.f, 1.f, 1.f));
	walkableTiles[148].walkableOffsets.push_back(glm::vec3(1.f, 2.f, 1.f));
	walkableTiles[148].walkableOffsets.push_back(glm::vec3(2.f, 3.f, 1.f));
	walkableTiles[148].walkableOffsets.push_back(glm::vec3(3.f, 4.f, 1.f));
	walkableTiles[148].walkableOffsets.push_back(glm::vec3(4.f, 5.f, 1.f));
	walkableTiles[148].unwalkableOffsets.push_back(glm::vec3(0.f, 0.f, 0.f));
	walkableTiles[148].unwalkableOffsets.push_back(glm::vec3(1.f, 0.f, 0.f));
	walkableTiles[148].unwalkableOffsets.push_back(glm::vec3(0.f, 0.f, 1.f));
	walkableTiles[148].unwalkableOffsets.push_back(glm::vec3(1.f, 0.f, 0.f));
	walkableTiles[148].unwalkableOffsets.push_back(glm::vec3(2.f, 0.f, 0.f));
	walkableTiles[148].unwalkableOffsets.push_back(glm::vec3(3.f, 0.f, 0.f));

	// mid stair5
	walkableTiles[149].walkableOffsets.push_back(glm::vec3(0.f, 1.f, 0.f));
	walkableTiles[149].walkableOffsets.push_back(glm::vec3(1.f, 2.f, 0.f));
	walkableTiles[149].walkableOffsets.push_back(glm::vec3(2.f, 3.f, 0.f));
	walkableTiles[149].walkableOffsets.push_back(glm::vec3(3.f, 4.f, 0.f));
	walkableTiles[149].walkableOffsets.push_back(glm::vec3(4.f, 5.f, 0.f));
	walkableTiles[149].unwalkableOffsets.push_back(glm::vec3(0.f, 0.f, 0.f));

	// left stair5
	walkableTiles[150].walkableOffsets.push_back(glm::vec3(0.f, 1.f, 0.f));
	walkableTiles[150].walkableOffsets.push_back(glm::vec3(1.f, 2.f, 0.f));
	walkableTiles[150].walkableOffsets.push_back(glm::vec3(2.f, 3.f, 0.f));
	walkableTiles[150].walkableOffsets.push_back(glm::vec3(3.f, 4.f, 0.f));
	walkableTiles[150].walkableOffsets.push_back(glm::vec3(4.f, 5.f, 0.f));
	walkableTiles[150].unwalkableOffsets.push_back(glm::vec3(0.f, 0.f, 0.f));
	walkableTiles[150].unwalkableOffsets.push_back(glm::vec3(0.f, 0.f, 1.f));
	walkableTiles[150].unwalkableOffsets.push_back(glm::vec3(1.f, 0.f, 1.f));
	walkableTiles[150].unwalkableOffsets.push_back(glm::vec3(2.f, 0.f, 1.f));
	walkableTiles[150].unwalkableOffsets.push_back(glm::vec3(3.f, 0.f, 1.f));
	walkableTiles[150].unwalkableOffsets.push_back(glm::vec3(4.f, 0.f, 1.f));

	// left stair
	walkableTiles[152].walkableOffsets.push_back(glm::vec3(0.f, 1.f, 0.f));
	walkableTiles[152].unwalkableOffsets.push_back(glm::vec3(0.f, 0.f, 0.f));

	walkableTiles[153].walkableOffsets.push_back(glm::vec3(0.f, 1.f, 0.f));
	walkableTiles[153].unwalkableOffsets.push_back(glm::vec3(0.f, 0.f, 0.f));

	walkableTiles[154].walkableOffsets.push_back(glm::vec3(0.f, 1.f, 0.f));
	walkableTiles[154].unwalkableOffsets.push_back(glm::vec3(0.f, 0.f, 0.f));

	walkableTiles[184].walkableOffsets.push_back(glm::vec3(1.f, 0.f, 0.f));
	walkableTiles[184].walkableOffsets.push_back(glm::vec3(2.f, 0.f, 0.f));
}

cMapManager::~cMapManager()
{	
	delete mapModel;

	for (std::map<int, sInstancedTile>::iterator it = instancedTiles.begin(); it != instancedTiles.end(); it++)
	{
		delete it->second.instancedModel;
		delete it->second.animation;
	}
}

void cMapManager::LoadMap(std::string mapModelName, std::string mapDescName)
{
	//g_set_Models.erase(mapModel);

    std::ifstream file("assets/models/WinterTest.pdsmap");

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

		// set quadrant coords
		file >> newQuad.quadZ;
		file >> newQuad.quadX;

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
				}
			}
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

		file >> currToken; // this should be mapend
		file >> currToken; // if there is another map, this will be mapstart
	}
	// end here

	for (std::map<int, sInstancedTile>::iterator it = instancedTiles.begin(); it != instancedTiles.end(); it++)
	{
		if (it->second.instanceOffsets.size() != 0)
		{
			it->second.instancedModel->InstanceObject(it->second.instanceOffsets, cRenderManager::GetInstance()->GetCurrentShaderId());
			cAnimationManager::GetInstance()->AddAnimation(it->second.animation);
			cRenderManager::GetInstance()->AddModel(it->second.instancedModel);

			it->second.instanceOffsets.clear();
		}
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
	desiredLocation.z += 15;

	int quadXCoord = ((int)desiredLocation.x) / 32;
	int quadZCoord = ((int)desiredLocation.z) / 32;

	for (std::vector<sQuadrant>::iterator it = quads.begin(); it != quads.end(); it++)
	{
		if (quadXCoord == it->quadX && quadZCoord == it->quadZ)
		{
			desiredLocation.z -= 32 * quadZCoord;
			desiredLocation.x -= 32 * quadXCoord;

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