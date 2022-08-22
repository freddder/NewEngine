#include "cShaderManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

cShaderManager::cShaderManager()
{
}

cShaderManager::~cShaderManager()
{
}

void cShaderManager::CreateShadderProgram(std::string programName, const char* vertexPath, const char* fragmentPath)
{
    if (programMap.count(programName) != 0) // it exists
    {
        std::cout << "This shadder program already exists" << std::endl;
        return;
    }

    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(SHADER_PATH + vertexPath);
        fShaderFile.open(SHADER_PATH + fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment;

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // shader Program
    unsigned int ID;
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    sShader newShader;
    newShader.ID = ID;

    programMap.insert(std::pair<std::string, sShader>(programName, newShader));
}

unsigned int cShaderManager::GetCurrentShaderId()
{
    return programMap[currShader].ID;
}

void cShaderManager::use(std::string programName)
{
    if (programMap.count(programName) == 0) // doesnt exists
        return;

    //currentProgramID = programMap[programName].ID;
    currShader = programName;
    glUseProgram(programMap[currShader].ID);
}

void cShaderManager::setBool(const std::string& name, bool value)
{
    if(programMap[currShader].uniformLocations.count(name) == 0)
    {
        unsigned int newLocation = glGetUniformLocation(programMap.at(currShader).ID, name.c_str());
        programMap[currShader].uniformLocations.insert(std::pair<std::string, unsigned int>(name, newLocation));
    }

    glUniform1i(programMap[currShader].uniformLocations[name], (int)value);
}

void cShaderManager::setInt(const std::string& name, int value)
{
    if (programMap[currShader].uniformLocations.count(name) == 0)
    {
        unsigned int newLocation = glGetUniformLocation(programMap.at(currShader).ID, name.c_str());
        programMap[currShader].uniformLocations.insert(std::pair<std::string, unsigned int>(name, newLocation));
    }

    glUniform1i(programMap[currShader].uniformLocations[name], value);
}

void cShaderManager::setFloat(const std::string& name, float value)
{
    if (programMap[currShader].uniformLocations.count(name) == 0)
    {
        unsigned int newLocation = glGetUniformLocation(programMap.at(currShader).ID, name.c_str());
        programMap[currShader].uniformLocations.insert(std::pair<std::string, unsigned int>(name, newLocation));
    }

    glUniform1f(programMap[currShader].uniformLocations[name], value);
}

void cShaderManager::setMat4(const std::string& name, const glm::mat4& mat)
{
    if (programMap[currShader].uniformLocations.count(name) == 0)
    {
        unsigned int newLocation = glGetUniformLocation(programMap.at(currShader).ID, name.c_str());
        programMap[currShader].uniformLocations.insert(std::pair<std::string, unsigned int>(name, newLocation));
    }

    glUniformMatrix4fv(programMap[currShader].uniformLocations[name], 1, GL_FALSE, &mat[0][0]);
}

void cShaderManager::setVec2(const std::string& name, const glm::vec2& value)
{
    if (programMap[currShader].uniformLocations.count(name) == 0)
    {
        unsigned int newLocation = glGetUniformLocation(programMap.at(currShader).ID, name.c_str());
        programMap[currShader].uniformLocations.insert(std::pair<std::string, unsigned int>(name, newLocation));
    }

    glUniform2fv(programMap[currShader].uniformLocations[name], 1, &value[0]);
}

void cShaderManager::setVec3(const std::string& name, const glm::vec3& value)
{
    if (programMap[currShader].uniformLocations.count(name) == 0)
    {
        unsigned int newLocation = glGetUniformLocation(programMap.at(currShader).ID, name.c_str());
        programMap[currShader].uniformLocations.insert(std::pair<std::string, unsigned int>(name, newLocation));
    }

    glUniform3fv(programMap[currShader].uniformLocations[name], 1, &value[0]);
}

void cShaderManager::setVec4(const std::string& name, const glm::vec4& value)
{
    if (programMap[currShader].uniformLocations.count(name) == 0)
    {
        unsigned int newLocation = glGetUniformLocation(programMap.at(currShader).ID, name.c_str());
        programMap[currShader].uniformLocations.insert(std::pair<std::string, unsigned int>(name, newLocation));
    }

    glUniform4fv(programMap[currShader].uniformLocations[name], 1, &value[0]);
}

void cShaderManager::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n ------------------------------------------------------- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n ------------------------------------------------------- " << std::endl;
        }
    }
}
