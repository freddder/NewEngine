#pragma once
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <map>

const std::string SHADER_PATH = "assets/shaders/";

struct sShader
{
    unsigned int ID;

    std::map<std::string, unsigned int> uniformLocations;
};

class cShaderManager
{
public:
    //unsigned int currentProgramID;

    cShaderManager();
    ~cShaderManager();

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

private:

    std::string currShader;
    std::map<std::string, sShader> programMap;

    // utility function for checking shader compilation/linking errors.
    void checkCompileErrors(unsigned int shader, std::string type);
};