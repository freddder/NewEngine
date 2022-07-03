#pragma once
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <map>

const std::string SHADER_PATH = "assets/shaders/";

class cShaderManager
{
public:
    unsigned int currentProgramID;

    cShaderManager();
    ~cShaderManager();

    void CreateShadderProgram(std::string programName, const char* vertexPath, const char* fragmentPath);

    void use(std::string programName);
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;

private:

    std::map<std::string, unsigned int> programMap;

    // utility function for checking shader compilation/linking errors.
    void checkCompileErrors(unsigned int shader, std::string type);
};