#include "cModelManager.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <glad/glad.h>

#include "cRenderManager.h"
#include "cTextureManager.h"

cModelManager* cModelManager::singleton = NULL;

cModelManager::cModelManager()
{
}

cModelManager::~cModelManager()
{
    for (std::map<std::string, sModelDrawInfo>::iterator it = modelMap.begin(); it != modelMap.end(); it++)
    {
        int size = (int)it->second.allMeshesData.size();

        // I dont wanna play with you anymore

        for (int i = 0; i < size; i++)
        {
            delete[] it->second.allMeshesData[i].verticesData;
            delete[] it->second.allMeshesData[i].indiciesData;

            glDeleteVertexArrays(1, &(it->second.allMeshesData[i].VAO_ID));
            glDeleteBuffers(1, &(it->second.allMeshesData[i].VBO_ID));
            glDeleteVertexArrays(1, &(it->second.allMeshesData[i].INDEX_ID));
        }
    }
}

bool cModelManager::LoadModel(std::string fileName, std::string programName)
{
    std::map< std::string, sModelDrawInfo>::iterator itDrawInfo = modelMap.find(fileName);

    if (itDrawInfo != modelMap.end()) // Find it? 
        return true; // already loaded

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

    if (scene->HasMeshes())
    {
        sModelDrawInfo newModel;

        newModel.numMeshes = scene->mNumMeshes;

        for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++) // per mesh
        {
            aiMesh* currMesh = scene->mMeshes[meshIndex];

            sMeshDrawInfo newMeshInfo;
            newMeshInfo.verticesData = new sVertexData[currMesh->mNumVertices];
            newMeshInfo.numberOfVertices = currMesh->mNumVertices;

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

                newMeshInfo.verticesData[vertexIndex] = newVertexInfo;
            
            } // end of per vertex

            newMeshInfo.numberOfIndices = currMesh->mNumFaces * 3;
            newMeshInfo.numberOfTriangles = currMesh->mNumFaces;

            newMeshInfo.indiciesData = new unsigned int[newMeshInfo.numberOfIndices];

            unsigned int indiceIndex = 0;
            for (unsigned int i = 0; i < currMesh->mNumFaces; i++)
            {
                aiFace currFace = currMesh->mFaces[i];
                for (unsigned int j = 0; j < currFace.mNumIndices; j++)
                {
                    newMeshInfo.indiciesData[indiceIndex + j] = currFace.mIndices[j];
                }
                indiceIndex += currFace.mNumIndices;
            }

            if (currMesh->mMaterialIndex > 0) // get texture for this mesh
            {
                aiMaterial* material = scene->mMaterials[currMesh->mMaterialIndex];

                aiString path;
                material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

                //std::string texture = path.C_Str();
                newMeshInfo.textureName = path.C_Str();

                // maybe try to load texture in the texture manager right here
                cTextureManager::GetInstance()->CreateTexture(newMeshInfo.textureName);
            }
        
            newModel.allMeshesData.push_back(newMeshInfo);
        } // end of per mesh

        cRenderManager::GetInstance()->use(programName);

        CreateModelVAOs(newModel, cRenderManager::GetInstance()->GetCurrentShaderId());
        modelMap.insert(std::pair<std::string, sModelDrawInfo>(fileName, newModel));

        return true;
    }

    return false;
}

bool cModelManager::FindModelByName(std::string fileName, sModelDrawInfo& modelInfo)
{
    std::map< std::string, sModelDrawInfo>::iterator itDrawInfo = modelMap.find(fileName);

    if (itDrawInfo == modelMap.end()) // Find it? 
        return false; // Nope

    modelInfo = itDrawInfo->second;
    return true;
}

std::string cModelManager::GetLastError(bool clear)
{
    std::string error = lastError;

    if (clear)
        lastError = "";

    return error;
}

void cModelManager::CreateModelVAOs(sModelDrawInfo& newModel, unsigned int program)
{
    for (unsigned int i = 0; i < newModel.numMeshes; i++)
    {
        // Generate VAO and VBO
        glGenVertexArrays(1, &(newModel.allMeshesData[i].VAO_ID));
        glGenBuffers(1, &(newModel.allMeshesData[i].VBO_ID));

        // Bind VAO and VBO
        glBindVertexArray(newModel.allMeshesData[i].VAO_ID);
        glBindBuffer(GL_ARRAY_BUFFER, newModel.allMeshesData[i].VBO_ID);

        // Specify VBO data
        glBufferData(GL_ARRAY_BUFFER, 
            sizeof(sVertexData) * newModel.allMeshesData[i].numberOfVertices, 
            (GLvoid*)newModel.allMeshesData[i].verticesData,
            GL_STATIC_DRAW);

        // Generate, bind and set indeces buffer
        glGenBuffers(1, &(newModel.allMeshesData[i].INDEX_ID));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newModel.allMeshesData[i].INDEX_ID);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            sizeof(unsigned int) * newModel.allMeshesData[i].numberOfIndices,
            (GLvoid*)newModel.allMeshesData[i].indiciesData,
            GL_STATIC_DRAW);

        //*************************************************************

        // Set vertex ins for shader

        // Position
        GLint vPos_location = glGetAttribLocation(program, "vPosition");
        glEnableVertexAttribArray(vPos_location);
        glVertexAttribPointer(vPos_location, 4,	
            GL_FLOAT, GL_FALSE,
            sizeof(sVertexData),
            (void*)offsetof(sVertexData, x));

        // Normal
        GLint vNormal_location = glGetAttribLocation(program, "vNormal");
        glEnableVertexAttribArray(vNormal_location);
        glVertexAttribPointer(vNormal_location, 4,
            GL_FLOAT, GL_FALSE,
            sizeof(sVertexData),
            (void*)offsetof(sVertexData, nx));

        // UVs
        GLint vUVx2_location = glGetAttribLocation(program, "vUVx2");
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
    }
}
