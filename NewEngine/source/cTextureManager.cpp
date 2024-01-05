#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <glad/glad.h>
#include <iostream>
#include "cTextureManager.h"
#include "cRenderManager.h"

cTextureManager* cTextureManager::singleton = NULL;

cTextureManager::cTextureManager()
{

}

unsigned int cTextureManager::CreateTexture(const std::string fullPath, int& width, int& height)
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

void cTextureManager::LoadGeneralTexture(const std::string fileName, const std::string subdirectory)
{
    if (generalTexturesMap.count(fileName) || fileName == "") return; // texture already created

    std::string fullPath = TEXTURE_PATH + subdirectory + fileName;
    int width, height;
    unsigned int textureId = CreateTexture(fullPath, width, height);

    if (textureId != 0)
        generalTexturesMap.insert(std::pair<std::string, unsigned int>(fileName, textureId));
}

bool cTextureManager::GetGeneralTexureId(const std::string texture, unsigned int& textureID)
{
    if (generalTexturesMap.find(texture) == generalTexturesMap.end()) // texture doesnt exists
        return false;

    textureID = generalTexturesMap[texture];
    return true;
}

unsigned int cTextureManager::CreateCubemap(std::vector<std::string> faces)
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

void cTextureManager::LoadSpriteSheet(const std::string spriteSheetName, unsigned int cols, unsigned int rows, bool sym, const std::string subdirectory)
{
    if (spriteSheetsMap.count(spriteSheetName)) return; // texture already created

    sSpriteSheet newSheet;
    newSheet.numCols = cols;
    newSheet.numRows = rows;
    newSheet.isSymmetrical = sym;

    std::string fullPath = TEXTURE_PATH + subdirectory + spriteSheetName;
    int width, height;
    newSheet.textureId = CreateTexture(fullPath, width, height);

    if (newSheet.textureId != 0)
        spriteSheetsMap[spriteSheetName] = newSheet;
}

void cTextureManager::SetupSpriteSheet(const std::string sheetName, const int spriteId)
{
    if (spriteSheetsMap.find(sheetName) == spriteSheetsMap.end()) return; // texture doesn't exists

    sSpriteSheet sheet = spriteSheetsMap[sheetName];

    cRenderManager* renderManager = cRenderManager::GetInstance();
    renderManager->setInt("spriteId", spriteId);
    renderManager->setInt("numCols", sheet.numCols);
    renderManager->setInt("numRows", sheet.numRows);
}

void cTextureManager::SetupTexture(std::string textureToSetup, unsigned int shaderTextureUnit)
{
    if (generalTexturesMap.find(textureToSetup) == generalTexturesMap.end()) return; // texture doesn't exists

    unsigned int textureId = generalTexturesMap[textureToSetup];

    std::string shaderVariable = "texture_" + std::to_string(shaderTextureUnit);

    //GLuint textureUnit = 0;			// Texture unit go from 0 to 79
    glActiveTexture(shaderTextureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
    glBindTexture(GL_TEXTURE_2D, textureId);

    cRenderManager::GetInstance()->setInt(shaderVariable, shaderTextureUnit);
}