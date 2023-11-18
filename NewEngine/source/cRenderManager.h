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

private:

    unsigned int notInstancedOffsetBufferId;

    unsigned int skyboxVAO, skyboxVBO;

    unsigned int depthMapID;
    unsigned int depthMapFBO;

    unsigned int cubemapTextureID;

    unsigned int uboMatricesID;
    unsigned int uboFogID;

    std::string currShader;
	std::map<std::string, sShaderProgram> programMap;

    void checkCompileErrors(unsigned int shader, std::string type);

    std::vector< std::shared_ptr<cRenderModel> > models;

    std::string lastError;
    void CreateModelVAOs(sModelDrawInfo& newModel, unsigned int program);

public:

	void CreateShadderProgram(std::string programName, const char* vertexPath, const char* fragmentPath);
    void use(std::string programName);
    unsigned int GetCurrentShaderId();
    unsigned int GetShaderIdByName(std::string programName);
    unsigned int GetDepthMapId();

    bool LoadModel(std::string fileName, std::string programName);
    bool FindModelByName(std::string fileName, std::string programName, sModelDrawInfo& modelInfo);
    std::string GetLastError(bool clear);

    void setBool(const std::string& name, bool value);
    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);
    void setMat4(const std::string& name, const glm::mat4& mat);
    void setVec2(const std::string& name, const glm::vec2& value);
    void setVec3(const std::string& name, const glm::vec3& value);
    void setVec4(const std::string& name, const glm::vec4& value);

    static std::shared_ptr<cRenderModel> CreateRenderModel();
    static std::shared_ptr<cSpriteModel> CreateSpriteModel();
    static std::shared_ptr<cAnimatedModel> CreateAnimatedModel(eAnimatedModels modelType);
    static void RemoveModel(std::shared_ptr<cRenderModel> model);
    void DrawObject(std::shared_ptr<cRenderModel> model);
    void DrawScene();

    // Debug quad
    unsigned int quadVAO;
    unsigned int quadVBO;
    unsigned int quadEBO;
};