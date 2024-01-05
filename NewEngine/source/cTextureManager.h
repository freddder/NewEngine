#pragma once

#include <string>
#include <map>
#include "cRenderModel.h"
#include "cAnimation.h"

const std::string TEXTURE_PATH = "assets/textures/";

struct sSpriteSheet
{
	unsigned int textureId;
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

	// Ideally this will be changed in the future so textures will be more easily managed
private:
	std::map<std::string, unsigned int> generalTexturesMap;
	unsigned int CreateTexture(const std::string fullPath, int& width, int& height);
public:
	void LoadGeneralTexture(const std::string fileName, const std::string subdirectory = "");
	bool GetGeneralTexureId(const std::string texture, unsigned int& textureID);
	unsigned int CreateCubemap(const std::vector<std::string> faces);

private:
	std::map<std::string, sSpriteSheet> spriteSheetsMap;
public:
	void LoadSpriteSheet(const std::string spriteSheetName, unsigned int cols, unsigned int rows, bool sym, const std::string subdirectory = "");
	void SetupSpriteSheet(const std::string sheetName, const int spriteId);

	void SetupTexture(std::string textureToSetup, unsigned int shaderTextureUnit = 0);
};