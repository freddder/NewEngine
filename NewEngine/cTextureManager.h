#pragma once

#include <string>
#include <map>
#include "cModel.h"

const std::string TEXTURE_PATH = "assets/textures/";

struct sSpriteSheet
{
	unsigned int numCols;
	unsigned int numRows;
	bool isSymetrical;
};

class cTextureManager
{
	std::map<std::string, sSpriteSheet> sheetsMap;
	std::map<std::string, unsigned int> texturesMap;

public:

	void CreateTexture(std::string fileName);
	void CreateSpriteSheet(std::string spriteSheetName, unsigned int cols, unsigned int rows, bool sym);
	void SetupTexture(std::string textureToSetup);
	void SetupSpriteSheet(std::string spriteSheetName);
	unsigned int CreateCubemap(std::vector<std::string> faces);

	bool GetSpritesheetSymetry(std::string spriteSheetName);
};