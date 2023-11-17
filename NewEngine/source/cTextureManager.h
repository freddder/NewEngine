#pragma once

#include <string>
#include <map>
#include "cRenderModel.h"
#include "cAnimation.h"

const std::string TEXTURE_PATH = "assets/textures/";

struct sSpriteSheet
{
	unsigned int numCols;
	unsigned int numRows;
	bool isSymmetrical;
};

class cTextureManager
{
	// Singleton
	static cTextureManager* singleton;
	cTextureManager();
	cTextureManager(const cTextureManager& obj) = delete;
public:
	static cTextureManager* GetInstance()
	{
		if (singleton == NULL)
		{
			singleton = new cTextureManager();
		}

		return singleton;
	}
	static void DestroyInstance()
	{
		if (singleton != NULL)
		{
			delete singleton;
			singleton = NULL;
		}
	}

private:
	std::map<std::string, sSpriteSheet> spriteSheetsMap;
	std::map<std::string, unsigned int> texturesMap;
public:
	void CreateTexture(std::string fileName);
	bool GetTexureId(std::string texture, unsigned int& textureID);
	unsigned int CreateCubemap(std::vector<std::string> faces);
	void CreateSpriteSheet(std::string spriteSheetName, unsigned int cols, unsigned int rows, bool sym);
	bool GetSpriteSheet(std::string sheetName, sSpriteSheet& sheet);

	void SetupTexture(std::string textureToSetup, unsigned int shaderTextureUnit);

	bool IsSpriteSymmetric(std::string spriteSheetName);
};