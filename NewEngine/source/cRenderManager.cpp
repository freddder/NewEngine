#include "cRenderManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Engine.h"
#include "cSceneManager.h"
#include "cLightManager.h"
#include "cCameraManager.h"
#include "cUIManager.h"

#include "cSpriteModel.h"
#include "cAnimatedModel.h"
#include "PokemonData.h"

#include "Player.h"

cRenderManager::cRenderManager()
{
}

cRenderManager::~cRenderManager()
{
}

void cRenderManager::Startup()
{
    Manager::camera.SCR_WIDTH = 1280;
    Manager::camera.SCR_HEIGHT = 720;

    //******* Create origin offset buffer for non instanced objects *************
    glm::vec4 originOffset = glm::vec4(0.f);

    glGenBuffers(1, &notInstancedOffsetBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, notInstancedOffsetBufferId);

    glBufferData(GL_ARRAY_BUFFER,
        sizeof(glm::vec4),
        (GLvoid*)&originOffset,
        GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //*************** Setup skybox vertices and VAOs ***************************
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // skybox VAO
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // load skybox textures
    std::vector<std::string> faces
    {
        "TropicalSunnyDayRight.jpg",    //"right.jpg",
        "TropicalSunnyDayLeft.jpg",     //"left.jpg",
        "TropicalSunnyDayUp.jpg",      //"top.jpg",
        "TropicalSunnyDayDown.jpg",   //"bottom.jpg",
        "TropicalSunnyDayFront.jpg",    //"front.jpg",
        "TropicalSunnyDayBack.jpg",     //"back.jpg"
    };
    cubemapTextureID = CreateCubemap(faces);

    //********************** Setup depth map FBO **********************
    glGenFramebuffers(1, &depthMapFBO);

    glGenTextures(1, &depthMapID);
    glBindTexture(GL_TEXTURE_2D, depthMapID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapID, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //*****************************************************************

    // setup matrices uniform block
    glGenBuffers(1, &uboMatricesID);

    glBindBuffer(GL_UNIFORM_BUFFER, uboMatricesID);
    glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4) + sizeof(int), NULL, GL_DYNAMIC_DRAW); // maybe change GL_STATIC_DRAW to dynamic
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatricesID, 0, 3 * sizeof(glm::mat4) + sizeof(int));

    // setup fog uniform block
    glGenBuffers(1, &uboFogID);

    glBindBuffer(GL_UNIFORM_BUFFER, uboFogID);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4) + 2 * sizeof(float), NULL, GL_DYNAMIC_DRAW); // maybe change GL_STATIC_DRAW to dynamic
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 2, uboFogID, 0, 2 * sizeof(glm::vec4) + 2 * sizeof(float));

    //********************** Setup on UI quad ****************************
    float quadVertices[] = {
        //  x,     y,    z, u(x), v(y)
         1.0f,  1.0f, 0.0f, 1.0f, 0.0f, // top right
         1.0f, -1.0f, 0.0f, 1.0f, 1.0f, // bottom right
        -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, // bottom left
        -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, // top left
    };

    unsigned int quadIndicies[] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    glGenVertexArrays(1, &UIQuadVAO);
    glGenBuffers(1, &UIQuadVBO);
    glGenBuffers(1, &UIQuadEBO);

    glBindVertexArray(UIQuadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, UIQuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, UIQuadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndicies), quadIndicies, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //***************************************************************************
}

void cRenderManager::Shutdown()
{
    // TODO: unload loaded models from shaders

    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
    glDeleteBuffers(1, &uboMatricesID);
    glDeleteBuffers(1, &uboFogID);
    glDeleteBuffers(1, &notInstancedOffsetBufferId);

    for (std::map<std::string, sFontData>::iterator it = fonts.begin(); it != fonts.end(); it++)
    {
        glDeleteBuffers(1, &it->second.textureAtlusId);
    }
}

void cRenderManager::CreateShadderProgram(std::string programName, const char* vertexPath, const char* fragmentPath)
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

    sShaderProgram newShader;
    newShader.ID = ID;

    programMap.insert(std::pair<std::string, sShaderProgram>(programName, newShader));

    // add Matrices block to matrices
    unsigned int ubMatricesIndex = glGetUniformBlockIndex(newShader.ID, "Matrices");
    glUniformBlockBinding(newShader.ID, ubMatricesIndex, 0);

    // add Lights block to matrices
    Manager::light.AddProgramToBlock(newShader.ID);

    // add Fog block to matrices
    unsigned int ubFogIndex = glGetUniformBlockIndex(newShader.ID, "Fog");
    glUniformBlockBinding(newShader.ID, ubFogIndex, 2);
}

unsigned int cRenderManager::GetCurrentShaderId()
{
    return programMap[currShader].ID;
}

unsigned int cRenderManager::GetDepthMapId()
{
    return depthMapID;
}

bool cRenderManager::LoadModel(std::string fileName, std::string programName)
{
    std::map<std::string, sShaderProgram>::iterator itPrograms = programMap.find(programName);
    if (itPrograms == programMap.end()) return false;

    std::map<std::string, sModelDrawInfo>::iterator itDrawInfo = programMap[programName].modelsLoaded.find(fileName);
    if (itDrawInfo != programMap[programName].modelsLoaded.end()) return true; // already loaded

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(MODEL_PATH + fileName,
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_PopulateArmatureData |
        aiProcess_FixInfacingNormals |
        aiProcess_LimitBoneWeights);

    if (scene == nullptr)
    {
        printf("ModelManager::LoadModel: ERROR: Failed to load file %s\n", fileName.c_str());
        printf(importer.GetErrorString());
        return false;
    }

    if (!scene->HasMeshes())
        return false;

    use(programName);

    sModelDrawInfo newModel;
    newModel.numMeshes = scene->mNumMeshes;

    for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++) // per mesh
    {
        aiMesh* currMesh = scene->mMeshes[meshIndex];

        sMeshDrawInfo newMeshInfo;
        newMeshInfo.numberOfVertices = currMesh->mNumVertices;
        sVertexData* verticesData = new sVertexData[currMesh->mNumVertices];

        for (unsigned int vertexIndex = 0; vertexIndex < currMesh->mNumVertices; vertexIndex++) // per vertex
        {
            sVertexData newVertexInfo;

            newVertexInfo.x = currMesh->mVertices[vertexIndex].x;
            newVertexInfo.y = currMesh->mVertices[vertexIndex].y;
            newVertexInfo.z = currMesh->mVertices[vertexIndex].z;
            newVertexInfo.w = 1;

            newVertexInfo.nx = currMesh->mNormals[vertexIndex].x;
            newVertexInfo.ny = currMesh->mNormals[vertexIndex].y;
            newVertexInfo.nz = currMesh->mNormals[vertexIndex].z;
            newVertexInfo.nw = 1;

            if (currMesh->mTextureCoords[0]) // if it has textures
            {
                newVertexInfo.u1 = currMesh->mTextureCoords[0][vertexIndex].x;
                newVertexInfo.v1 = 1.f - currMesh->mTextureCoords[0][vertexIndex].y;
                newVertexInfo.u2 = 0;
                newVertexInfo.v2 = 0;
            }
            //else
            //{
            //    newVertexInfo.u1 =  0.f;
            //    newVertexInfo.v1 =  0.f;
            //    newVertexInfo.u2 =  0.f;
            //    newVertexInfo.v2 =  0.f;
            //}

            //newVertexInfo.r = 1.f;
            //newVertexInfo.g = 1.f;
            //newVertexInfo.b = 1.f;
            //newVertexInfo.a = 1.f;

            verticesData[vertexIndex] = newVertexInfo;

        } // end of per vertex

        newMeshInfo.numberOfIndices = currMesh->mNumFaces * 3;
        newMeshInfo.numberOfTriangles = currMesh->mNumFaces;

        unsigned int* indiciesData = new unsigned int[newMeshInfo.numberOfIndices];

        unsigned int indiceIndex = 0;
        for (unsigned int i = 0; i < currMesh->mNumFaces; i++)
        {
            aiFace currFace = currMesh->mFaces[i];
            for (unsigned int j = 0; j < currFace.mNumIndices; j++)
            {
                indiciesData[indiceIndex + j] = currFace.mIndices[j];
            }
            indiceIndex += currFace.mNumIndices;
        }

        if (currMesh->mMaterialIndex > 0) // get texture for this mesh
        {
            aiMaterial* material = scene->mMaterials[currMesh->mMaterialIndex];

            aiString path;
            material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
            newMeshInfo.textureName = path.C_Str();

            // maybe try to load texture right here
            LoadMapTexture(newMeshInfo.textureName);
        }

#pragma region VAO_Creation

        // Generate VAO and VBO
        glGenVertexArrays(1, &(newMeshInfo.VAO_ID));
        glGenBuffers(1, &(newMeshInfo.VBO_ID));

        // Bind VAO and VBO
        glBindVertexArray(newMeshInfo.VAO_ID);
        glBindBuffer(GL_ARRAY_BUFFER, newMeshInfo.VBO_ID);

        // Specify VBO data
        glBufferData(GL_ARRAY_BUFFER,
            sizeof(sVertexData) * newMeshInfo.numberOfVertices,
            (GLvoid*)verticesData,
            GL_STATIC_DRAW);

        // Generate, bind and set indeces buffer
        glGenBuffers(1, &(newMeshInfo.INDEX_ID));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMeshInfo.INDEX_ID);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            sizeof(unsigned int) * newMeshInfo.numberOfIndices,
            (GLvoid*)indiciesData,
            GL_STATIC_DRAW);

        unsigned int shaderID = GetCurrentShaderId();

        // Set vertex ins for shader

        // Position
        GLint vPos_location = glGetAttribLocation(shaderID, "vPosition");
        glEnableVertexAttribArray(vPos_location);
        glVertexAttribPointer(vPos_location, 4,
            GL_FLOAT, GL_FALSE,
            sizeof(sVertexData),
            (void*)offsetof(sVertexData, x));

        // Normal
        GLint vNormal_location = glGetAttribLocation(shaderID, "vNormal");
        glEnableVertexAttribArray(vNormal_location);
        glVertexAttribPointer(vNormal_location, 4,
            GL_FLOAT, GL_FALSE,
            sizeof(sVertexData),
            (void*)offsetof(sVertexData, nx));

        // UVs
        GLint vUVx2_location = glGetAttribLocation(shaderID, "vUVx2");
        glEnableVertexAttribArray(vUVx2_location);
        glVertexAttribPointer(vUVx2_location, 4,
            GL_FLOAT, GL_FALSE,
            sizeof(sVertexData),
            (void*)offsetof(sVertexData, u1));

        // Unbind buffers
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glDisableVertexAttribArray(vPos_location);
        glDisableVertexAttribArray(vNormal_location);
        glDisableVertexAttribArray(vUVx2_location);

#pragma endregion VAO_Creation

        delete[] verticesData;
        delete[] indiciesData;

        newModel.allMeshesData.push_back(newMeshInfo);
    } // end of per mesh

    programMap[programName].modelsLoaded.insert(std::pair<std::string, sModelDrawInfo>(fileName, newModel));

    return true;
}

bool cRenderManager::FindModelByName(std::string fileName, std::string programName, sModelDrawInfo& modelInfo)
{
    std::map<std::string, sShaderProgram>::iterator itProgram = programMap.find(programName);

    if (itProgram == programMap.end()) return false; // Didn't find it

    std::map<std::string, sModelDrawInfo>::iterator itDrawInfo = programMap[programName].modelsLoaded.find(fileName);

    if (itDrawInfo == programMap[programName].modelsLoaded.end()) return false; // Didn't find it

    modelInfo = itDrawInfo->second;
    return true;
}

void cRenderManager::checkCompileErrors(unsigned int shader, std::string type)
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

void cRenderManager::use(std::string programName)
{
    if (programMap.count(programName) == 0) return; // Doesn't exists

    currShader = programName;
    glUseProgram(programMap[currShader].ID);
}

void cRenderManager::setBool(const std::string& name, bool value)
{
    if (programMap[currShader].uniformLocations.count(name) == 0)
    {
        unsigned int newLocation = glGetUniformLocation(programMap.at(currShader).ID, name.c_str());
        programMap[currShader].uniformLocations.insert(std::pair<std::string, unsigned int>(name, newLocation));
    }

    glUniform1i(programMap[currShader].uniformLocations[name], (int)value);
}

void cRenderManager::setInt(const std::string& name, int value)
{
    if (programMap[currShader].uniformLocations.count(name) == 0)
    {
        unsigned int newLocation = glGetUniformLocation(programMap.at(currShader).ID, name.c_str());
        programMap[currShader].uniformLocations.insert(std::pair<std::string, unsigned int>(name, newLocation));
    }

    glUniform1i(programMap[currShader].uniformLocations[name], value);
}

void cRenderManager::setFloat(const std::string& name, float value)
{
    if (programMap[currShader].uniformLocations.count(name) == 0)
    {
        unsigned int newLocation = glGetUniformLocation(programMap.at(currShader).ID, name.c_str());
        programMap[currShader].uniformLocations.insert(std::pair<std::string, unsigned int>(name, newLocation));
    }

    glUniform1f(programMap[currShader].uniformLocations[name], value);
}

void cRenderManager::setMat4(const std::string& name, const glm::mat4& mat)
{
    if (programMap[currShader].uniformLocations.count(name) == 0)
    {
        unsigned int newLocation = glGetUniformLocation(programMap.at(currShader).ID, name.c_str());
        programMap[currShader].uniformLocations.insert(std::pair<std::string, unsigned int>(name, newLocation));
    }

    glUniformMatrix4fv(programMap[currShader].uniformLocations[name], 1, GL_FALSE, &mat[0][0]);
}

void cRenderManager::setVec2(const std::string& name, const glm::vec2& value)
{
    if (programMap[currShader].uniformLocations.count(name) == 0)
    {
        unsigned int newLocation = glGetUniformLocation(programMap.at(currShader).ID, name.c_str());
        programMap[currShader].uniformLocations.insert(std::pair<std::string, unsigned int>(name, newLocation));
    }

    glUniform2fv(programMap[currShader].uniformLocations[name], 1, &value[0]);
}

void cRenderManager::setVec3(const std::string& name, const glm::vec3& value)
{
    if (programMap[currShader].uniformLocations.count(name) == 0)
    {
        unsigned int newLocation = glGetUniformLocation(programMap.at(currShader).ID, name.c_str());
        programMap[currShader].uniformLocations.insert(std::pair<std::string, unsigned int>(name, newLocation));
    }

    glUniform3fv(programMap[currShader].uniformLocations[name], 1, &value[0]);
}

void cRenderManager::setVec4(const std::string& name, const glm::vec4& value)
{
    if (programMap[currShader].uniformLocations.count(name) == 0)
    {
        unsigned int newLocation = glGetUniformLocation(programMap.at(currShader).ID, name.c_str());
        programMap[currShader].uniformLocations.insert(std::pair<std::string, unsigned int>(name, newLocation));
    }

    glUniform4fv(programMap[currShader].uniformLocations[name], 1, &value[0]);
}

std::shared_ptr<cRenderModel> cRenderManager::CreateMapRenderModel()
{
    std::shared_ptr<cRenderModel> newModel = std::make_shared<cRenderModel>();
    mapModels.push_back(newModel);

    return newModel;
}

std::shared_ptr<cSpriteModel> cRenderManager::CreateSpriteModel()
{
    std::shared_ptr<cSpriteModel> newModel = std::make_shared<cSpriteModel>();
    mapModels.push_back(newModel);

    return newModel;
}

std::shared_ptr<cAnimatedModel> cRenderManager::CreateMapAnimatedModel(eAnimatedModel modelType)
{
    std::shared_ptr<cAnimatedModel> newModel;

    switch (modelType)
    {
    case FOAM:
        newModel = std::make_shared<cFoamModel>();
        break;
    case OCEAN:
        newModel = std::make_shared<cOceanModel>();
        break;
    case WAVE:
        newModel = std::make_shared<cWaveModel>();
        break;
    case TREE:
        newModel = std::make_shared<cTreeModel>();
        break;
    }
    mapModels.push_back(newModel);

    return newModel;
}

void cRenderManager::RemoveMapModel(std::shared_ptr<cRenderModel> model)
{
    std::vector< std::shared_ptr<cRenderModel> >::iterator it = std::find(mapModels.begin(), mapModels.end(), model);
    
    if(it != mapModels.end())
        mapModels.erase(it);
}

unsigned int cRenderManager::CreateTexture(const std::string fullPath, int& width, int& height)
{
    unsigned int textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // load and generate the texture
    int nrChannels;
    unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        //std::cout << "Created texture " << fileName << std::endl;
    }
    else
    {
        glDeleteTextures(1, &textureId);
        std::cout << "Failed to load texture " << fullPath << std::endl;
        return 0;
    }

    stbi_image_free(data);
    return textureId;
}

void cRenderManager::LoadMapTexture(const std::string fileName, const std::string subdirectory)
{
    if (fileName == "") return;

    if (mapTextures.count(fileName)) return;// texture already created

    std::string fullPath = TEXTURE_PATH + subdirectory + fileName;
    int width, height;
    sTexture newTexture;
    newTexture.textureId = CreateTexture(fullPath, width, height);

    if (newTexture.textureId != 0)
        mapTextures.insert(std::pair<std::string, sTexture>(fileName, newTexture));
}

unsigned int cRenderManager::CreateCubemap(const std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        std::string fullPath = TEXTURE_PATH + "skyboxes/" + faces[i];
        unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void cRenderManager::LoadRoamingPokemonFormSpriteSheet(const int nationalDexId, const std::string formTag)
{
    std::string textureName = std::to_string(nationalDexId);
    if (formTag != "")
    {
        textureName = textureName + "_" + formTag;
    }
    std::string shinyTextureName = textureName + "_s.png";
    textureName = textureName + ".png";

    // Check if not already loaded
    if (sceneSpriteSheets.find(textureName) != sceneSpriteSheets.end()) return;

    std::string dexIdString = std::to_string(nationalDexId);
    while (dexIdString.length() < 4)
    {
        dexIdString = "0" + dexIdString;
    }
    std::string texturePath = PKM_SPRITES_PATH + dexIdString + "/";

    // Create sprite sheet
    sSpriteSheet newSheet;
    newSheet.numCols = 4;
    newSheet.numRows = 4;
    newSheet.isSymmetrical = false;

    std::string fullPath = texturePath + textureName;
    int width, height;
    newSheet.textureId = CreateTexture(fullPath, width, height);

    if (newSheet.textureId != 0)
        sceneSpriteSheets.insert(std::pair<std::string, sSpriteSheet>(textureName, newSheet));

    // Check if shiny not already loaded
    if (sceneSpriteSheets.find(shinyTextureName) != sceneSpriteSheets.end()) return;

    // Create shiny sprite sheet
    sSpriteSheet newShinySheet;
    newShinySheet.numCols = 4;
    newShinySheet.numRows = 4;
    newShinySheet.isSymmetrical = false;

    std::string shinyFullPath = texturePath + shinyTextureName;
    newShinySheet.textureId = CreateTexture(shinyFullPath, width, height);

    if (newShinySheet.textureId != 0)
        sceneSpriteSheets.insert(std::pair<std::string, sSpriteSheet>(shinyTextureName, newShinySheet));
}

void cRenderManager::LoadSpriteSheet(const std::string spriteSheetName, unsigned int cols, unsigned int rows, bool sym, const std::string subdirectory)
{
    if (sceneSpriteSheets.count(spriteSheetName)) return; // texture already created

    sSpriteSheet newSheet;
    newSheet.numCols = cols;
    newSheet.numRows = rows;
    newSheet.isSymmetrical = sym;

    std::string fullPath = TEXTURE_PATH + subdirectory + spriteSheetName;
    int width, height;
    newSheet.textureId = CreateTexture(fullPath, width, height);

    if (newSheet.textureId != 0)
        sceneSpriteSheets[spriteSheetName] = newSheet;
}

void cRenderManager::LoadRoamingPokemonSpecieSpriteSheets(const Pokemon::sSpeciesData& specieData)
{
    // Load default form
    LoadRoamingPokemonFormSpriteSheet(specieData.nationalDexNumber);

    // Load female varient if there is one
    if (specieData.isSpriteGenderBased || specieData.isFormGenderBased)
    {
        LoadRoamingPokemonFormSpriteSheet(specieData.nationalDexNumber, "f");
    }
    else
    {
        // Load all alternate forms
        for (std::map<std::string, Pokemon::sForm>::const_iterator it = specieData.alternateForms.cbegin(); it != specieData.alternateForms.cend(); it++)
        {
            LoadRoamingPokemonFormSpriteSheet(specieData.nationalDexNumber, it->first);
        }
    }
}

void cRenderManager::SetupSpriteSheet(const std::string sheetName, const int spriteId, const unsigned int shaderTextureUnit)
{
    if (sceneSpriteSheets.find(sheetName) == sceneSpriteSheets.end()) return; // texture doesn't exists

    sSpriteSheet sheet = sceneSpriteSheets[sheetName];

    setInt("spriteId", spriteId);
    setInt("numCols", sheet.numCols);
    setInt("numRows", sheet.numRows);

    unsigned int textureId = sceneSpriteSheets[sheetName].textureId;

    //GLuint textureUnit = 0;			// Texture unit go from 0 to 79
    glActiveTexture(shaderTextureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
    glBindTexture(GL_TEXTURE_2D, textureId);

    std::string shaderVariable = "texture_" + std::to_string(shaderTextureUnit);
    setInt(shaderVariable, shaderTextureUnit);
}

void cRenderManager::SetupTexture(const std::string textureToSetup, const unsigned int shaderTextureUnit)
{
    if (mapTextures.find(textureToSetup) == mapTextures.end()) return; // texture doesn't exists

    unsigned int textureId = mapTextures[textureToSetup].textureId;

    //GLuint textureUnit = 0;			// Texture unit go from 0 to 79
    glActiveTexture(shaderTextureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
    glBindTexture(GL_TEXTURE_2D, textureId);

    std::string shaderVariable = "texture_" + std::to_string(shaderTextureUnit);
    setInt(shaderVariable, shaderTextureUnit);
}

void cRenderManager::LoadFont(const std::string fontName, const unsigned int glyphSize)
{
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR: Could not init FreeType Library" << std::endl;
        return;
    }

    std::string fontPath = FONTS_PATH + fontName;
    FT_Face face;
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        std::cout << "ERROR: Failed to load font" << std::endl;
        return;
    }

    // Leave width as 0 to be calculated with height
    FT_Set_Pixel_Sizes(face, 0, glyphSize);

    sFontData newFont;
    newFont.glyphSize = glyphSize;
    // Create empty black texture
    glGenTextures(1, &newFont.textureAtlusId);
    glBindTexture(GL_TEXTURE_2D, newFont.textureAtlusId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, FONT_ATLAS_COLS * glyphSize, FONT_ATLAS_ROWS * glyphSize, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Fill slots of atlus with individual characters
    int i = 0;
    for (unsigned char c = 32; c < 126; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR: Failed to load Glyph " << c << std::endl;
            continue;
        }

        sFontCharData newChar;
        newChar.size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
        newChar.bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
        newChar.advance = face->glyph->advance.x;

        // No need to add space to texture
        if (c != ' ')
        {
            glTexSubImage2D(GL_TEXTURE_2D, 0,
                glyphSize * (i % FONT_ATLAS_COLS),
                glyphSize * (i / FONT_ATLAS_COLS),
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                GL_RED, GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );

            i++;
        }

        newFont.characters.insert(std::pair<char, sFontCharData>(c, newChar));
    }

    fonts.insert(std::pair<std::string, sFontData>(fontName, newFont));

    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void cRenderManager::DrawObject(std::shared_ptr<cRenderModel> model)
{
    sModelDrawInfo drawInfo;
    if (!FindModelByName(model->meshName, model->shaderName, drawInfo)) return;

    use(model->shaderName);
    
    setVec3("modelPosition", model->position);
    setMat4("modelOrientationX", glm::rotate(glm::mat4(1.0f), model->orientation.x, glm::vec3(1.f, 0.f, 0.f)));
    setMat4("modelOrientationY", glm::rotate(glm::mat4(1.0f), model->orientation.y, glm::vec3(0.f, 1.f, 0.f)));
    setMat4("modelOrientationZ", glm::rotate(glm::mat4(1.0f), model->orientation.z, glm::vec3(0.f, 0.f, 1.f)));
    setMat4("modelScale", glm::scale(glm::mat4(1.0f), model->scale));

    setBool("useWholeColor", model->useWholeColor);
    setVec4("wholeColor", model->wholeColor);

    model->SetUpUniforms();

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMapID);
    setInt("shadowMap", 1);

    for (unsigned int i = 0; i < drawInfo.allMeshesData.size(); i++)
    {
        if (model->textureName == "") SetupTexture(drawInfo.allMeshesData[i].textureName);

        // Bind VAO
        glBindVertexArray(drawInfo.allMeshesData[i].VAO_ID);

        // Check for instanced
        if (model->isInstanced)
        {
            glBindBuffer(GL_ARRAY_BUFFER, model->instanceOffsetsBufferId);

            // OPTMIZATION: maybe figure out a way to not have to setup all these data every frame
            GLint offset_location = glGetAttribLocation(programMap[currShader].ID, "oOffset");
            glEnableVertexAttribArray(offset_location);
            glVertexAttribPointer(offset_location, 4,
                GL_FLOAT, GL_FALSE,
                sizeof(glm::vec4),
                (void*)0);
            glVertexAttribDivisor(offset_location, 1);

            glDrawElementsInstanced(GL_TRIANGLES,
                drawInfo.allMeshesData[i].numberOfIndices,
                GL_UNSIGNED_INT,
                (void*)0,
                model->instancedNum);
        }
        else
        {
            glBindBuffer(GL_ARRAY_BUFFER, notInstancedOffsetBufferId);

            GLint offset_location = glGetAttribLocation(programMap[currShader].ID, "oOffset");
            glEnableVertexAttribArray(offset_location);
            glVertexAttribPointer(offset_location, 4,
                GL_FLOAT, GL_FALSE,
                sizeof(glm::vec4),
                (void*)0);
            glVertexAttribDivisor(offset_location, 1);

            glDrawElements(GL_TRIANGLES,
                drawInfo.allMeshesData[i].numberOfIndices,
                GL_UNSIGNED_INT,
                (void*)0);
        }

        glBindVertexArray(0);
    }    
}

void cRenderManager::SetupFont(const std::string fontName)
{
    if (fonts.find(fontName) == fonts.end()) return; // font doesn't exists

    unsigned int textureId = fonts[fontName].textureAtlusId;

    glActiveTexture(GL_TEXTURE0);	// GL_TEXTURE0 = 33984
    glBindTexture(GL_TEXTURE_2D, textureId);

    std::string shaderVariable = "texture_0";
    setInt(shaderVariable, 0);

    setInt("atlasRowsNum", FONT_ATLAS_ROWS);
    setInt("atlasColsNum", FONT_ATLAS_COLS);
}

void cRenderManager::CreateTextDataBuffer(cUIText* text)
{
    if (fonts.find(text->fontName) == fonts.end()) return; // font doesn't exists
    sFontData& font = fonts[text->fontName];

    std::stringstream ss(text->text);
    std::vector<std::string> words;
    std::string s;
    while (std::getline(ss, s, ' '))
    {
        words.push_back(s); // no spaces
    }

    float glyphPixelRatio = text->CalculateHeightPixels() * text->textSizePercent / (float)font.glyphSize;
    float pixelCutoff = text->CalculateWidthPixels();

    std::vector<sCharBufferData> data;
    int advanceX = 0;
    int advanceY = 0;
    for (unsigned int i = 0; i < words.size(); i++)
    {
        // Check if this word is too big for this line
        int wordAdvance = 0;
        for (unsigned int j = 0; j < words[i].length(); j++)
        {
            sFontCharData& ch = font.characters[words[i][j]];
            wordAdvance += ch.advance >> 6;
        }

        // Jump to next line
        if ((advanceX + wordAdvance) * glyphPixelRatio > pixelCutoff)
        {
            advanceX = 0;
            advanceY += font.glyphSize * 1.1f;
        }
        
        for (unsigned int j = 0; j < words[i].length(); j++)
        {
            char c = words[i][j];
            sFontCharData& ch = font.characters[c];

            int posX = advanceX + ch.bearing.x;
            int posY = ch.size.y - ch.bearing.y + advanceY;

            int sizeX = ch.size.x;
            int sizeY = ch.size.y;

            sCharBufferData newData;
            newData.posX = (float)posX;
            newData.posY = (float)posY;
            newData.sizeX = (float)sizeX;
            newData.sizeY = (float)sizeY;
            newData.charId = (float)c;
            data.push_back(newData);

            advanceX += ch.advance >> 6;
        }
        advanceX += font.characters[' '].advance >> 6;
    }

    text->drawCharCount = data.size();

    glGenBuffers(1, &text->dataBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, text->dataBufferId);

    glBufferData(GL_ARRAY_BUFFER,
        sizeof(sCharBufferData) * data.size(),
        (GLvoid*)&data[0],
        GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void cRenderManager::DrawText(cUIText* textWidget)
{
    sFontData& font = fonts[textWidget->fontName];
    unsigned int scrWidth = Manager::camera.SCR_WIDTH;
    unsigned int scrHeight = Manager::camera.SCR_HEIGHT;
    float horizontalTranslation = textWidget->CalculateHorizontalTranslate();
    float verticalTranslation = textWidget->CalculateVerticalTranslate();
    float widthPercent = textWidget->CalculateWidthScreenPercent();
    float heightPercent = textWidget->CalculateHeightScreenPercent();

    float pixelGlyphRatio = heightPercent * scrHeight * textWidget->textSizePercent / (float)font.glyphSize;

    float finalHorizontalTranslation = horizontalTranslation - widthPercent;
    float finalVerticalTranslation = verticalTranslation + heightPercent;
    glm::vec2 origin = glm::vec2(finalHorizontalTranslation, finalVerticalTranslation);

    use("text");
    SetupFont(textWidget->fontName);
    setVec3("color", textWidget->color);

    setVec2("originOffset", origin);
    setFloat("glyphPixelRatio", pixelGlyphRatio);
    setInt("glyphSize", font.glyphSize);
    setInt("screenWidth", scrWidth);
    setInt("screenHeight", scrHeight);

    glBindVertexArray(UIQuadVAO);

    // Setup buffer data as vertex atribute
    // (ideally I would want this to be set on VAO creation, but I guess the data needs to be setup before hand... so here it goes)
    glBindBuffer(GL_ARRAY_BUFFER, textWidget->dataBufferId);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribDivisor(2, 1);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(4 * sizeof(float)));
    glVertexAttribDivisor(3, 1);

    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0, textWidget->drawCharCount);

    glBindVertexArray(0);
}

void cRenderManager::DrawParticles(cParticleSpawner* spawner)
{
    sModelDrawInfo drawInfo;
    if (!FindModelByName(spawner->model.meshName, spawner->model.shaderName, drawInfo)) return;
    
    use(spawner->model.shaderName);
    setVec3("cameraPosition", Manager::camera.position);
    setMat4("modelScale", glm::scale(glm::mat4(1.0f), spawner->model.scale));
    setBool("useWholeColor", spawner->model.useWholeColor);
    setVec4("wholeColor", spawner->model.wholeColor);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, GetDepthMapId());
    setInt("shadowMap", 1);
    
    // Might change this to use a constant quad instead of a custom mesh
    for (unsigned int i = 0; i < drawInfo.allMeshesData.size(); i++)
    {
        // Setup texture
        std::string textureToUse = spawner->model.textureName;
        SetupTexture(textureToUse);
    
        // Bind VAO
        glBindVertexArray(drawInfo.allMeshesData[i].VAO_ID);
    
        glBindBuffer(GL_ARRAY_BUFFER, spawner->particleBufferId);
    
        GLint offset_location = glGetAttribLocation(GetCurrentShaderId(), "oOffset");
        glEnableVertexAttribArray(offset_location);
        glVertexAttribPointer(offset_location, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
        glVertexAttribDivisor(offset_location, 1);
    
        glDrawElementsInstanced(GL_TRIANGLES,
            drawInfo.allMeshesData[i].numberOfIndices,
            GL_UNSIGNED_INT,
            (void*)0,
            spawner->particles.size());
    
        glBindVertexArray(0);
    }
}

void cRenderManager::DrawWidget(cUIWidget* widget)
{
    for (int i = 0; i < widget->children.size(); i++)
    {
        cUIWidget* child = widget->children[i];

        if (child->ShouldUseTextRenderer()) DrawText(dynamic_cast<cUIText*>(child));
        else DrawWidget(child);
    }
    
    widget->SetupWidget();

    float widthPercent = widget->CalculateWidthScreenPercent();
    float heightPercent = widget->CalculateHeightScreenPercent();
    setFloat("widthPercent", widthPercent);
    setFloat("heightPercent", heightPercent);

    float widthTranslate = widget->CalculateHorizontalTranslate();
    float heightTranslate = widget->CalculateVerticalTranslate(); 
    setFloat("widthTranslate", widthTranslate);
    setFloat("heightTranslate", heightTranslate);

    glBindVertexArray(UIQuadVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void cRenderManager::DrawFrame()
{
    //********************** Shadow pass ********************************

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    glm::mat4 lightProjection, lightView;
    float near_plane = 1.f, far_plane = 100.f;

    glm::vec3 lightPos = glm::vec3(Manager::light.lights[0].position) + Player::GetPlayerPosition();
    glm::vec3 lightAt = Player::GetPlayerPosition();

    lightProjection = glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, near_plane, far_plane);
    lightView = glm::lookAt(lightPos, lightAt, glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;

    int bruh = 1; // B R U H

    glBindBuffer(GL_UNIFORM_BUFFER, uboMatricesID);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(lightProjection));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(lightView));
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(lightSpaceMatrix));
    glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), sizeof(int), &bruh);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    //Draw scene
    for (int i = 0; i < mapModels.size(); i++)
    {
        DrawObject(mapModels[i]);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //*********************** Regular pass ******************************
    // Reset viewport
    glViewport(0, 0, Manager::camera.SCR_WIDTH, Manager::camera.SCR_HEIGHT);
    glClearColor(0.89f, 0.89f, 0.89f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Manager::light.SetUnimormValues();

    // Set camera and fog UBOs
    glm::mat4 projection = Manager::camera.GetProjectionMatrix();
    glm::mat4 view = Manager::camera.GetViewMatrix();

    bruh = 0;
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatricesID);
    glBufferSubData(GL_UNIFORM_BUFFER, 0 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
    glBufferSubData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(lightSpaceMatrix));
    glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), sizeof(int), &bruh);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBuffer(GL_UNIFORM_BUFFER, uboFogID);
    glBufferSubData(GL_UNIFORM_BUFFER, 0 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(glm::vec4(*Manager::camera.targetPosRef, 1.f)));
    glBufferSubData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(glm::vec4(Manager::scene.fogColor, 1.f)));
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), sizeof(float), &Manager::scene.fogDensity);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4) + sizeof(float), sizeof(float), &Manager::scene.fogGradient);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Draw scene
    for (int i = 0; i < mapModels.size(); i++)
    {
        DrawObject(mapModels[i]);
    }

    // Draw particles
    if (Manager::scene.weatherParticleSpawner)
    {
        DrawParticles(Manager::scene.weatherParticleSpawner);
    }
    for (int i = 0; i < Manager::scene.particleSpawners.size(); i++)
    {
        DrawParticles(Manager::scene.particleSpawners[i]);
    }

    // Draw UI
    if (!Manager::ui.canvases.empty())
    {
        const cUICanvas* canvasToDraw = Manager::ui.canvases.top();

        for (int i = 0; i < canvasToDraw->anchoredWidgets.size(); i++)
        {
            if (canvasToDraw->anchoredWidgets[i]->ShouldUseTextRenderer()) DrawText(dynamic_cast<cUIText*>(canvasToDraw->anchoredWidgets[i]));
            else DrawWidget(canvasToDraw->anchoredWidgets[i]);
        }
    }

    // Draw skybox
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    use("skybox");
    view = glm::mat4(glm::mat3(Manager::camera.GetViewMatrix())); // remove translation from the view matrix
    projection = Manager::camera.GetProjectionMatrix();

    setMat4("view", view);
    setMat4("projection", projection);

    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default      
}