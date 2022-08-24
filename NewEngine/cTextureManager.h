#pragma once

#include <string>
#include <map>
#include "cRenderModel.h"

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
	bool GetTexureId(std::string texture, unsigned int& textureID);
	bool GetSpriteSheet(std::string sheetName, sSpriteSheet& sheet);
	void SetupTexture(std::string textureToSetup);
	//void SetupSpriteSheet(std::string spriteSheetName);
	unsigned int CreateCubemap(std::vector<std::string> faces);

	bool GetSpritesheetSymetry(std::string spriteSheetName);
};