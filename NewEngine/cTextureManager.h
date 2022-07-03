#pragma once

#include <string>
#include <map>
#include "cModel.h"

const std::string TEXTURE_PATH = "assets/textures/";

class cTextureManager
{
	std::map<std::string, unsigned int> texturesMap;

public:

	void CreateTexture(std::string fileName);
	unsigned int CreateCubemap(std::vector<std::string> faces);
	void SetupTexture(std::string textureToSetup);
};