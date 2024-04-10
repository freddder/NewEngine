#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <set>
#include <map>
#include <memory>
#include "DrawInfo.h"
#include "cRenderModel.h"

const std::string SHADER_PATH = "assets/shaders/";
const std::string MODEL_PATH = "assets/models/";
const std::string TEXTURE_PATH = "assets/textures/";

const unsigned int SHADOW_WIDTH = 3048, SHADOW_HEIGHT = 3048;

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
    bool isPermanent = false; // if false, deleted when scene changes 
};

struct sSpriteSheet : sTexture
{
    unsigned int numCols;
    unsigned int numRows;
    bool isSymmetrical;
};

class cRenderManager
{
    static cRenderManager* singleton;
    cRenderManager();
    ~cRenderManager();
    cRenderManager(const cRenderManager& obj) = delete;
public:
    static cRenderManager* GetInstance()
    {
        if (singleton == NULL)
        {
            singleton = new cRenderManager();
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

    // Shaders
private:
    std::string currShader;
    std::map<std::string, sShaderProgram> programMap;
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

    // UI quad
private:
    unsigned int UIQuadVAO;
    unsigned int UIQuadVBO;
    unsigned int UIQuadEBO;

    // Models loading
private:
    unsigned int notInstancedOffsetBufferId;
public:
    bool LoadModel(std::string fileName, std::string programName);
    bool FindModelByName(std::string fileName, std::string programName, sModelDrawInfo& modelInfo);

    // Scene models
private:
    std::vector< std::shared_ptr<cRenderModel> > models;
public:
    static std::shared_ptr<cRenderModel> CreateRenderModel();
    static std::shared_ptr<class cSpriteModel> CreateSpriteModel();
    static std::shared_ptr<class cAnimatedModel> CreateAnimatedModel(eAnimatedModel modelType);
    static void RemoveModel(std::shared_ptr<cRenderModel> model);

    // Textures
private:
    std::map<std::string, sTexture> sceneTextures;
    unsigned int CreateTexture(const std::string fullPath, int& width, int& height);
public:
    void LoadSceneTexture(const std::string fileName, const std::string subdirectory = "", bool isPermanent = false);
    unsigned int CreateCubemap(const std::vector<std::string> faces);

private:
    std::map<std::string, sSpriteSheet> sceneSpriteSheets;
public:
    void LoadSpriteSheet(const std::string spriteSheetName, unsigned int cols, unsigned int rows, bool sym = false, const std::string subdirectory = "", bool isPermanent = false);
    void SetupSpriteSheet(const std::string sheetName, const int spriteId, const unsigned int shaderTextureUnit = 0);

    void SetupTexture(const std::string textureToSetup, const unsigned int shaderTextureUnit = 0);

    // Drawing
private:
    void DrawObject(std::shared_ptr<cRenderModel> model);
    void DrawParticles(class cParticleSpawner* spawner);
    void DrawWidget(class cUIWidget* widget);
public:
    void DrawFrame();
};