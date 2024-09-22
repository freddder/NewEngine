#pragma once
#include <glm/glm.hpp>
#include <set>
#include <map>
#include <memory>
#include "DrawInfo.h"
#include "cRenderModel.h"

namespace Pokemon
{
    struct sSpeciesData;
    struct sIndividualData;
}

struct sShaderProgram
{
    unsigned int ID;
    std::map<std::string, sModelDrawInfo> modelsLoaded; // stored by file name
    std::map<std::string, unsigned int> uniformLocations;
};

enum eAnimatedModel
{
    OCEAN,  // 0
    FOAM,   // 1
    WAVE,   // 2
    TREE    // 3
};

struct sTexture
{
    unsigned int textureId;
};

struct sSpriteSheet : sTexture
{
public:
    unsigned int numCols;
    unsigned int numRows;
    bool isSymmetrical;
};

class cRenderManager
{
public:
    cRenderManager();
    ~cRenderManager();

    void Startup();
    void Shutdown();

    // Shaders
private:
    std::string currShader;
    std::map<std::string, sShaderProgram> programs;
    void checkCompileErrors(unsigned int shader, std::string type);
public:
    void CreateShadderProgram(std::string programName, const char* vertexPath, const char* fragmentPath);
    unsigned int GetCurrentShaderId();
    void use(std::string programName);
    void setBool(const std::string& name, bool value);
    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);
    void setMat4(const std::string& name, const glm::mat4& mat);
    void setVec2(const std::string& name, const glm::vec2& value);
    void setVec3(const std::string& name, const glm::vec3& value);
    void setVec4(const std::string& name, const glm::vec4& value);

    // Depth map
private:
    unsigned int depthMapID, depthMapFBO;
public:
    unsigned int GetDepthMapId();

    // Skybox
private:
    unsigned int skyboxVAO, skyboxVBO;
    unsigned int cubemapTextureID;

    // Uniform Buffer Objects
private:
    unsigned int uboMatricesID;
    unsigned int uboFogID;

    // Models loading
private:
    unsigned int notInstancedOffsetBufferId;
    bool FindModelByName(std::string fileName, std::string programName, sModelDrawInfo& modelInfo);
public:
    bool LoadModel(std::string fileName, std::string programName);

    // Render models
private:
    std::vector< std::shared_ptr<cRenderModel> > mapModels;
    std::vector< std::shared_ptr<cRenderModel> > battleModels; // prob not the best idea
public:
    std::shared_ptr<cRenderModel> CreateRenderModel(bool isBattleModel = false);
    std::shared_ptr<class cSpriteModel> CreateSpriteModel(bool isBattleModel = false);
    std::shared_ptr<class cAnimatedModel> CreateAnimatedModel(eAnimatedModel modelType, bool isBattleModel = false);
    void RemoveModel(std::shared_ptr<cRenderModel> model);

    // Textures
private:
    std::map<std::string, sTexture> textures;
    unsigned int CreateTexture(const std::string fullPath, int& width, int& height);
public:
    void LoadTexture(const std::string fileName, const std::string subdirectory = "");
    void UnloadTextures();
    unsigned int CreateCubemap(const std::vector<std::string> faces); // TEMP

private:
    std::map<std::string, sSpriteSheet> spriteSheets;
    void LoadRoamingPokemonFormSpriteSheet(const int nationalDexId, const std::string formTag = "");
    void LoadPokemonIconTexture(const int nationalDexId, const std::string formTag = "");
public:
    void LoadSpriteSheet(const std::string spriteSheetName, unsigned int cols, unsigned int rows, bool sym = false, const std::string subdirectory = "");
    void LoadRoamingPokemonSpecieTextures(const Pokemon::sSpeciesData& specieData);
    float LoadPokemonBattleSpriteSheet(Pokemon::sIndividualData& data, bool isFront = true); // kinda wanted to make this const but whatever

    void SetupSpriteSheet(const std::string sheetName, const int spriteId, const unsigned int shaderTextureUnit = 0);
    void SetupTexture(const std::string textureToSetup, const unsigned int shaderTextureUnit = 0);

    // Drawing
private:
    void DrawObject(std::shared_ptr<cRenderModel> model);
    void DrawParticles(class cParticleSpawner* spawner);
    void DrawShadowPass(glm::mat4& outLightSpaceMatrix);
public:
    void DrawFrame();

    friend class cUICanvas;
};