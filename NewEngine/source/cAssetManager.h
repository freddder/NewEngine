#pragma once

#include <map>
#include <string>
#include "DrawInfo.h"

struct sShaderProgram
{
    unsigned int ID;

    std::map<std::string, sModelDrawInfo> permanentLoadedModels; // stored by file name
    std::map<std::string, sModelDrawInfo> temporaryLoadedModels; // maybe a good idea?

    std::map<std::string, unsigned int> uniformLocations;
};

struct sSpriteSheet
{
    unsigned int textureId;
    unsigned int numCols;
    unsigned int numRows;
    bool isSymmetrical;
};

class cAssetManager
{
    static cAssetManager* singleton;
    cAssetManager();
    ~cAssetManager();
    cAssetManager(const cAssetManager& obj) = delete;
public:
    static cAssetManager* GetInstance()
    {
        if (singleton == NULL)
        {
            singleton = new cAssetManager();
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

    // Shaders & models
private:
    sShaderProgram generalModelShader; // From simple rendering (used for basic map and static objects)
    std::map<std::string, sShaderProgram> customShaders; // For animated models
    void CreateModelVAOs(sModelDrawInfo& newModel, unsigned int program);
public:
    bool LoadModel(const std::string fileName, const std::string programName = "");
    bool UnloadModel(const std::string fileName, const std::string programName = "");

    // UI
private:
    unsigned int UIQuadVAO, UIQuadVBO, UIQuadEBO;
    sShaderProgram uiShader;

    // Textures
private:
    std::map<std::string, unsigned int> mapTextures;
    std::map<std::string, unsigned int> uiTextures;
    sSpriteSheet playerSpriteSheet;
    std::map<std::string, sSpriteSheet> spriteSheets;
    unsigned int CreateTexture(const std::string fullPath, int& width, int& height);
public:
    void LoadMapTexture(const std::string fileName, const std::string subdirectory = "");
    void LoadSpriteSheet(const std::string spriteSheetName, unsigned int cols, unsigned int rows, bool sym, const std::string subdirectory = "");
    void LoadUITexture(const std::string fileName);
    void UnloadMapTexture(const std::string fileName);
    void UnloadSpriteSheet(const std::string fileName);
    void UnloadUITexture(const std::string fileName);

    // Cubemap
private:
    unsigned int cubemapTextureID;
    unsigned int skyboxVAO, skyboxVBO;
public:
    void CreateCubemap(const std::vector<std::string> faces);
};