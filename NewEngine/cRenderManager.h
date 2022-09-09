#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <set>
#include <map>
#include "cRenderModel.h"

const std::string SHADER_PATH = "assets/shaders/";

const unsigned int SHADOW_WIDTH = 3048, SHADOW_HEIGHT = 3048;

struct sShaderProgram
{
    unsigned int ID;
    std::map<std::string, unsigned int> uniformLocations;
};

class cRenderManager
{
    unsigned int notInstancedOffsetBufferId;

    unsigned int skyboxVAO, skyboxVBO;

    unsigned int depthMapID;
    unsigned int depthMapFBO;

    unsigned int cubemapTextureID;

    unsigned int uboMatricesID;

    std::string currShader;
	std::map<std::string, sShaderProgram> programMap;

    void checkCompileErrors(unsigned int shader, std::string type);

    std::set< cRenderModel* > models;

public:

	cRenderManager();
	~cRenderManager();

	void CreateShadderProgram(std::string programName, const char* vertexPath, const char* fragmentPath);
    void use(std::string programName);
    unsigned int GetCurrentShaderId();
    unsigned int GetDepthMapId();

    void setBool(const std::string& name, bool value);
    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);
    void setMat4(const std::string& name, const glm::mat4& mat);
    void setVec2(const std::string& name, const glm::vec2& value);
    void setVec3(const std::string& name, const glm::vec3& value);
    void setVec4(const std::string& name, const glm::vec4& value);

    void AddModel(cRenderModel* newModel);
    void RemoveModel(cRenderModel* model);
    void DrawObject(cRenderModel* model);
    void DrawScene();
};