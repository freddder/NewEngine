#include "cRenderManager.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <tracy/tracy/Tracy.hpp>

#include "Engine.h"
#include "cSceneManager.h"
#include "cLightManager.h"
#include "cCameraManager.h"
#include "cUIManager.h"
#include "cInputManager.h"

#include "cSpriteModel.h"
#include "cAnimatedModel.h"
#include "PokemonData.h"

#include "Player.h"

const std::string SHADER_PATH = "assets/shaders/";
const std::string MODEL_PATH = "assets/models/";
const std::string TEXTURE_PATH = "assets/textures/";
const std::string PKM_DATA_PATH = "assets/pokemon/";

const unsigned int SHADOW_WIDTH = 3048, SHADOW_HEIGHT = 3048;

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

    // Setup shader programs
    CreateShaderProgram("scene", "VertShader1.glsl", "FragShader1.glsl");
    CreateShaderProgram("skybox", "SkyboxVertShader.glsl", "SkyboxFragShader.glsl");
    CreateShaderProgram("sprite", "SpriteVertShader.glsl", "FragShader1.glsl");
    CreateShaderProgram("wave", "WaveVertShader.glsl", "WaveFragShader.glsl");
    CreateShaderProgram("ocean", "OceanVertShader.glsl", "OceanFragShader.glsl");
    CreateShaderProgram("foam", "FoamVertShader.glsl", "FoamFragShader.glsl");
    CreateShaderProgram("tree", "TreeVertShader.glsl", "FragShader1.glsl");
    CreateShaderProgram("snow", "SnowVertShader.glsl", "SnowFragShader.glsl");
    CreateShaderProgram("particle", "3DParticleVertShader.glsl", "FragShader1.glsl");
    CreateShaderProgram("ui", "UIVertShader.glsl", "UIFragShader.glsl");
    CreateShaderProgram("text", "TextVertShader.glsl", "TextFragShader.glsl");

    // Tracy screenshot setup
    glGenTextures(4, m_fiTexture);
    glGenFramebuffers(4, m_fiFramebuffer);
    glGenBuffers(4, m_fiPbo);
    for (int i = 0; i < 4; i++)
    {
        glBindTexture(GL_TEXTURE_2D, m_fiTexture[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 320, 180, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glBindFramebuffer(GL_FRAMEBUFFER, m_fiFramebuffer[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fiTexture[i], 0);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, m_fiPbo[i]);
        glBufferData(GL_PIXEL_PACK_BUFFER, 320 * 180 * 4, nullptr, GL_STREAM_READ);
    }
}

void cRenderManager::Shutdown()
{
    // TODO: unload loaded models from shaders and textures

    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
    glDeleteBuffers(1, &uboMatricesID);
    glDeleteBuffers(1, &uboFogID);
    glDeleteBuffers(1, &notInstancedOffsetBufferId);

    UnloadTextures();
    mapModels.clear();
    battleModels.clear();
}

void cRenderManager::CreateShaderProgram(std::string programName, const char* vertexPath, const char* fragmentPath)
{
    if (programs.count(programName) != 0) // it exists
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

    programs.insert(std::pair<std::string, sShaderProgram>(programName, newShader));

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
    return programs[currShader].ID;
}

unsigned int cRenderManager::GetDepthMapId()
{
    return depthMapID;
}

bool cRenderManager::LoadModel(std::string fileName, std::string programName)
{
    std::map<std::string, sShaderProgram>::iterator itPrograms = programs.find(programName);
    if (itPrograms == programs.end()) return false;

    std::map<std::string, sModelDrawInfo>::iterator itDrawInfo = programs[programName].modelsLoaded.find(fileName);
    if (itDrawInfo != programs[programName].modelsLoaded.end()) return true; // already loaded

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

            // Load texture
            LoadTexture(newMeshInfo.textureName);
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

    programs[programName].modelsLoaded.insert(std::pair<std::string, sModelDrawInfo>(fileName, newModel));

    return true;
}

void cRenderManager::UnloadModels()
{
    for (auto itPrograms = programs.begin(); itPrograms != programs.end(); itPrograms++)
    {
        for (auto itModels = itPrograms->second.modelsLoaded.begin(); itModels != itPrograms->second.modelsLoaded.end(); itModels++)
        {
            for (int i = 0; i < itModels->second.allMeshesData.size(); i++)
            {
                glDeleteVertexArrays(1, &itModels->second.allMeshesData[i].VAO_ID);
                glDeleteBuffers(1, &itModels->second.allMeshesData[i].VBO_ID);
            }

            itModels->second.allMeshesData.clear();
        }

        itPrograms->second.modelsLoaded.clear();
    }
}

bool cRenderManager::FindModelByName(std::string fileName, std::string programName, sModelDrawInfo& modelInfo)
{
    if (programs.find(programName) == programs.end()) return false; // Didn't find it

    std::map<std::string, sModelDrawInfo>::iterator itDrawInfo = programs[programName].modelsLoaded.find(fileName);

    if (itDrawInfo == programs[programName].modelsLoaded.end()) return false; // Didn't find it

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
    if (programs.count(programName) == 0) return; // Doesn't exists

    currShader = programName;
    glUseProgram(programs[currShader].ID);
}

void cRenderManager::setBool(const std::string& name, bool value)
{
    if (programs[currShader].uniformLocations.count(name) == 0)
    {
        unsigned int newLocation = glGetUniformLocation(programs.at(currShader).ID, name.c_str());
        programs[currShader].uniformLocations.insert(std::pair<std::string, unsigned int>(name, newLocation));
    }

    glUniform1i(programs[currShader].uniformLocations[name], (int)value);
}

void cRenderManager::setInt(const std::string& name, int value)
{
    if (programs[currShader].uniformLocations.count(name) == 0)
    {
        unsigned int newLocation = glGetUniformLocation(programs.at(currShader).ID, name.c_str());
        programs[currShader].uniformLocations.insert(std::pair<std::string, unsigned int>(name, newLocation));
    }

    glUniform1i(programs[currShader].uniformLocations[name], value);
}

void cRenderManager::setFloat(const std::string& name, float value)
{
    if (programs[currShader].uniformLocations.count(name) == 0)
    {
        unsigned int newLocation = glGetUniformLocation(programs.at(currShader).ID, name.c_str());
        programs[currShader].uniformLocations.insert(std::pair<std::string, unsigned int>(name, newLocation));
    }

    glUniform1f(programs[currShader].uniformLocations[name], value);
}

void cRenderManager::setMat4(const std::string& name, const glm::mat4& mat)
{
    if (programs[currShader].uniformLocations.count(name) == 0)
    {
        unsigned int newLocation = glGetUniformLocation(programs.at(currShader).ID, name.c_str());
        programs[currShader].uniformLocations.insert(std::pair<std::string, unsigned int>(name, newLocation));
    }

    glUniformMatrix4fv(programs[currShader].uniformLocations[name], 1, GL_FALSE, &mat[0][0]);
}

void cRenderManager::setVec2(const std::string& name, const glm::vec2& value)
{
    if (programs[currShader].uniformLocations.count(name) == 0)
    {
        unsigned int newLocation = glGetUniformLocation(programs.at(currShader).ID, name.c_str());
        programs[currShader].uniformLocations.insert(std::pair<std::string, unsigned int>(name, newLocation));
    }

    glUniform2fv(programs[currShader].uniformLocations[name], 1, &value[0]);
}

void cRenderManager::setVec3(const std::string& name, const glm::vec3& value)
{
    if (programs[currShader].uniformLocations.count(name) == 0)
    {
        unsigned int newLocation = glGetUniformLocation(programs.at(currShader).ID, name.c_str());
        programs[currShader].uniformLocations.insert(std::pair<std::string, unsigned int>(name, newLocation));
    }

    glUniform3fv(programs[currShader].uniformLocations[name], 1, &value[0]);
}

void cRenderManager::setVec4(const std::string& name, const glm::vec4& value)
{
    if (programs[currShader].uniformLocations.count(name) == 0)
    {
        unsigned int newLocation = glGetUniformLocation(programs.at(currShader).ID, name.c_str());
        programs[currShader].uniformLocations.insert(std::pair<std::string, unsigned int>(name, newLocation));
    }

    glUniform4fv(programs[currShader].uniformLocations[name], 1, &value[0]);
}

std::shared_ptr<cRenderModel> cRenderManager::CreateRenderModel(bool isBattleModel)
{
    std::shared_ptr<cRenderModel> newModel = std::make_shared<cRenderModel>();

    if (isBattleModel) 
        battleModels.push_back(newModel);
    else 
        mapModels.push_back(newModel);

    return newModel;
}

std::shared_ptr<cSpriteModel> cRenderManager::CreateSpriteModel(bool isBattleModel)
{
    std::shared_ptr<cSpriteModel> newModel = std::make_shared<cSpriteModel>();

    if (isBattleModel) 
        battleModels.push_back(newModel);
    else 
        mapModels.push_back(newModel);

    return newModel;
}

std::shared_ptr<cAnimatedModel> cRenderManager::CreateAnimatedModel(eAnimatedModel modelType, bool isBattleModel)
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

    if (isBattleModel)
        battleModels.push_back(newModel);
    else
        mapModels.push_back(newModel);

    return newModel;
}

void cRenderManager::RemoveModel(std::shared_ptr<cRenderModel> model)
{
    if (Engine::currGameMode == eGameMode::MAP) // TEMP
    {
        std::vector< std::shared_ptr<cRenderModel> >::iterator it = std::find(mapModels.begin(), mapModels.end(), model);

        if (it != mapModels.end())
            mapModels.erase(it);
    }
    else
    {
        std::vector< std::shared_ptr<cRenderModel> >::iterator it = std::find(battleModels.begin(), battleModels.end(), model);

        if (it != battleModels.end())
            battleModels.erase(it);
    }
}

unsigned int cRenderManager::CreateTexture(const std::string fullPath, int& width, int& height)
{
    // load and generate the texture
    int nrChannels;
    unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &nrChannels, 0);
    if (!data)
    {
        std::cout << "Failed to load texture " << fullPath << std::endl;
        return 0;
    }

    unsigned int textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    return textureId;
}

void cRenderManager::LoadTexture(const std::string fileName, const std::string subdirectory)
{
    if (fileName == "") return;

    if (textures.find(fileName) != textures.end()) return; // texture already created

    std::string fullPath = TEXTURE_PATH + subdirectory + fileName;
    int width, height;
    sTexture newTexture;
    newTexture.textureId = CreateTexture(fullPath, width, height);

    if (newTexture.textureId != 0)
    {
        textures.insert(std::pair<std::string, sTexture>(fileName, newTexture));
    }
}

void cRenderManager::UnloadTextures()
{
    for (std::map<std::string, sTexture>::iterator it = textures.begin(); it != textures.end(); it++)
    {
        glDeleteTextures(1, &it->second.textureId);
    }
    textures.clear();

    for (std::map<std::string, sSpriteSheet>::iterator it = spriteSheets.begin(); it != spriteSheets.end(); it++)
    {
        glDeleteTextures(1, &it->second.textureId);
    }
    spriteSheets.clear();
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
    if (spriteSheets.find(textureName) != spriteSheets.end()) return;

    std::string dexIdString = Pokemon::MakeDexNumberFolderName(nationalDexId);
    std::string texturePath = PKM_DATA_PATH + dexIdString + "/";

    // Create sprite sheet
    sSpriteSheet newSheet;
    newSheet.numCols = 4;
    newSheet.numRows = 4;
    newSheet.isSymmetrical = false;

    std::string fullPath = texturePath + textureName;
    int width, height;
    newSheet.textureId = CreateTexture(fullPath, width, height);

    if (newSheet.textureId != 0)
        spriteSheets.insert(std::pair<std::string, sSpriteSheet>(textureName, newSheet));

    // Check if shiny not already loaded
    if (spriteSheets.find(shinyTextureName) != spriteSheets.end()) return;

    // Create shiny sprite sheet
    sSpriteSheet newShinySheet;
    newShinySheet.numCols = 4;
    newShinySheet.numRows = 4;
    newShinySheet.isSymmetrical = false;

    std::string shinyFullPath = texturePath + shinyTextureName;
    newShinySheet.textureId = CreateTexture(shinyFullPath, width, height);

    if (newShinySheet.textureId != 0)
        spriteSheets.insert(std::pair<std::string, sSpriteSheet>(shinyTextureName, newShinySheet));
}

void cRenderManager::LoadSpriteSheet(const std::string spriteSheetName, unsigned int cols, unsigned int rows, bool sym, const std::string subdirectory)
{
    if (spriteSheets.count(spriteSheetName)) return; // texture already created

    sSpriteSheet newSheet;
    newSheet.numCols = cols;
    newSheet.numRows = rows;
    newSheet.isSymmetrical = sym;

    std::string fullPath = TEXTURE_PATH + subdirectory + spriteSheetName;
    int width, height;
    newSheet.textureId = CreateTexture(fullPath, width, height);

    if (newSheet.textureId != 0)
        spriteSheets[spriteSheetName] = newSheet;
}

void cRenderManager::LoadRoamingPokemonSpecieTextures(const Pokemon::sSpeciesData& specieData)
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

float cRenderManager::LoadPokemonBattleSpriteSheet(Pokemon::sIndividualData& data, bool isFront)
{
    std::string textureName = data.MakeBattleTextureName(isFront);

    if (textures.find(textureName) != textures.end()) return 1.f; // already loaded

    std::string dexIdString = std::to_string(data.nationalDexNumber);
    while (dexIdString.length() < 4)
    {
        dexIdString = "0" + dexIdString;
    }
    std::string texturePath = PKM_DATA_PATH + dexIdString + "/";

    std::string fullPath = texturePath + textureName;
    sSpriteSheet newSpriteSheet;
    newSpriteSheet.numRows = 1;
    newSpriteSheet.numCols = isFront ? data.form.battleFrontSpriteFrameCount : data.form.battleBackSpriteFrameCount;

    int width, height;
    newSpriteSheet.textureId = CreateTexture(fullPath, width, height);

    spriteSheets.insert(std::pair<std::string, sSpriteSheet>(textureName, newSpriteSheet));

    return (float)width / newSpriteSheet.numCols / height;
}

void cRenderManager::SetupSpriteSheet(const std::string sheetName, const int spriteId, const unsigned int shaderTextureUnit)
{
    sSpriteSheet sheet = spriteSheets[sheetName];

    setInt("spriteId", spriteId);
    setInt("numCols", sheet.numCols);
    setInt("numRows", sheet.numRows);

    unsigned int textureId = sheet.textureId;

    //GLuint textureUnit = 0;			// Texture unit go from 0 to 79
    glActiveTexture(shaderTextureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
    glBindTexture(GL_TEXTURE_2D, textureId);

    std::string shaderVariable = "texture_" + std::to_string(shaderTextureUnit);
    setInt(shaderVariable, shaderTextureUnit);
}

void cRenderManager::SetupTexture(const std::string textureToSetup, const unsigned int shaderTextureUnit)
{
    if (textures.find(textureToSetup) == textures.end())
    {
        std::cout << "Failed to setup texture: " << textureToSetup << std::endl;
        return; // texture doesn't exists
    }

    unsigned int textureId = textures[textureToSetup].textureId;

    //GLuint textureUnit = 0;			// Texture unit go from 0 to 79
    glActiveTexture(shaderTextureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
    glBindTexture(GL_TEXTURE_2D, textureId);

    std::string shaderVariable = "texture_" + std::to_string(shaderTextureUnit);
    setInt(shaderVariable, shaderTextureUnit);
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
            GLint offset_location = glGetAttribLocation(programs[currShader].ID, "oOffset");
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

            GLint offset_location = glGetAttribLocation(programs[currShader].ID, "oOffset");
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

void cRenderManager::DrawParticles(cParticleSpawner* spawner)
{
    sModelDrawInfo drawInfo;
    if (!FindModelByName(spawner->model.meshName, spawner->model.shaderName, drawInfo)) return;
    
    use(spawner->model.shaderName);
    setVec3("cameraPosition", Manager::camera.position);
    setVec3("modelScale", spawner->model.scale);
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

void cRenderManager::DrawShadowPass(glm::mat4& outLightSpaceMatrix)
{
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    glm::mat4 lightProjection, lightView;
    float near_plane = 1.f, far_plane = 100.f;

    glm::vec3 lightPos, lightAt;
    if (Engine::currGameMode == eGameMode::MAP)
    {
        lightPos = glm::vec3(Manager::light.lights[0].position) + Player::GetPlayerPosition();
        lightAt = Player::GetPlayerPosition();
    }
    else if (Engine::currGameMode == eGameMode::BATTLE)
    {
        lightPos = glm::vec3(-20.f, 12.f, -10.f);
        lightAt = glm::vec3(0.f); // look at world origin
    }

    lightProjection = glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, near_plane, far_plane);
    lightView = glm::lookAt(lightPos, lightAt, glm::vec3(0.0, 1.0, 0.0));
    outLightSpaceMatrix = lightProjection * lightView;

    int bruh = 1; // B R U H
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatricesID);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(lightProjection));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(lightView));
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(outLightSpaceMatrix));
    glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), sizeof(int), &bruh);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    //Draw scene
    if (Engine::currGameMode == eGameMode::MAP)
    {
        for (int i = 0; i < mapModels.size(); i++)
        {
            DrawObject(mapModels[i]);
        }
    }
    else if (Engine::currGameMode == eGameMode::BATTLE)
    {
        for (int i = 0; i < battleModels.size(); i++)
        {
            DrawObject(battleModels[i]);
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void cRenderManager::SendTracyScreenshot()
{
    while (!m_fiQueue.empty())
    {
        const auto fiIdx = m_fiQueue.front();
        if (glClientWaitSync(m_fiFence[fiIdx], 0, 0) == GL_TIMEOUT_EXPIRED) break;
        glDeleteSync(m_fiFence[fiIdx]);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, m_fiPbo[fiIdx]);
        auto ptr = glMapBufferRange(GL_PIXEL_PACK_BUFFER, 0, 320 * 180 * 4, GL_MAP_READ_BIT);
        FrameImage(ptr, 320, 180, m_fiQueue.size(), true);
        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
        m_fiQueue.erase(m_fiQueue.begin());
    }

    assert(m_fiQueue.empty() || m_fiQueue.front() != m_fiIdx); // check for buffer overrun
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fiFramebuffer[m_fiIdx]);
    glBlitFramebuffer(0, 0, Manager::camera.SCR_WIDTH, Manager::camera.SCR_HEIGHT, 0, 0, 320, 180, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fiFramebuffer[m_fiIdx]);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, m_fiPbo[m_fiIdx]);
    glReadPixels(0, 0, 320, 180, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    m_fiFence[m_fiIdx] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    m_fiQueue.emplace_back(m_fiIdx);
    m_fiIdx = (m_fiIdx + 1) % 4;
}

void cRenderManager::DrawFrame()
{
    ZoneScoped;

    //Shadow pass
    glm::mat4 lightSpaceMatrix;
    DrawShadowPass(lightSpaceMatrix);

    // Regular pass
    
    // Reset viewport
    glViewport(0, 0, Manager::camera.SCR_WIDTH, Manager::camera.SCR_HEIGHT);
    glClearColor(0.89f, 0.89f, 0.89f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Manager::light.SetUnimormValues();

    // Set camera and fog UBOs
    glm::mat4 projection = Manager::camera.GetProjectionMatrix();
    glm::mat4 view = Manager::camera.GetViewMatrix();

    int bruh = 0;
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
    if (Engine::currGameMode == eGameMode::MAP)
    {
        for (int i = 0; i < mapModels.size(); i++)
        {
            DrawObject(mapModels[i]);
        }
    }
    else if (Engine::currGameMode == eGameMode::BATTLE)
    {
        for (int i = 0; i < battleModels.size(); i++)
        {
            DrawObject(battleModels[i]);
        }
    }

    // Draw particles
    if (Engine::currGameMode != eGameMode::MENU)
    {
        if (Manager::scene.weatherParticleSpawner)
        {
            DrawParticles(Manager::scene.weatherParticleSpawner);
        }

        for (int i = 0; i < Manager::scene.particleSpawners.size(); i++)
        {
            DrawParticles(Manager::scene.particleSpawners[i].get());
        }
    }

    // Draw UI
    if (Manager::input.GetCurrentInputState() == MENU_NAVIGATION)
        Manager::ui.DrawUI();

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

    FrameMark;
}
