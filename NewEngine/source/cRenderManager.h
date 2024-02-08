#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <set>
#include <map>
#include <memory>
#include "DrawInfo.h"
#include "cRenderModel.h"

class cSpriteModel;
class cAnimatedModel;
class cParticleSpawner;
class cUIWidget;

const std::string SHADER_PATH = "assets/shaders/";
const std::string MODEL_PATH = "assets/models/";

const unsigned int SHADOW_WIDTH = 3048, SHADOW_HEIGHT = 3048;

struct sShaderProgram
{
    unsigned int ID;
    std::map<std::string, sModelDrawInfo> modelsLoaded; // stored by file name
    std::map<std::string, unsigned int> uniformLocations;
};

enum eAnimatedModels
{
    OCEAN,
    FOAM,
    WAVE
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
    void CreateModelVAOs(sModelDrawInfo& newModel, unsigned int program);
public:
    bool LoadModel(std::string fileName, std::string programName);
    bool FindModelByName(std::string fileName, std::string programName, sModelDrawInfo& modelInfo);

private:
    std::vector< std::shared_ptr<cRenderModel> > models;
public:
    static std::shared_ptr<cRenderModel> CreateRenderModel();
    static std::shared_ptr<cSpriteModel> CreateSpriteModel();
    static std::shared_ptr<cAnimatedModel> CreateAnimatedModel(eAnimatedModels modelType);
    static void RemoveModel(std::shared_ptr<cRenderModel> model);

    // Drawing
private:
    void DrawObject(std::shared_ptr<cRenderModel> model);
    void DrawParticles();
    void DrawWidget(cUIWidget* widget);
public:
    void DrawFrame();
};