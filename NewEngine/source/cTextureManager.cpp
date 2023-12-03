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

void cTextureManager::CreateTexture(std::string fileName, std::string subdirectory)
{
    if (texturesMap.count(fileName) || fileName == "") // texture already created
        return;

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
    int width, height, nrChannels;
    std::string fullPath = TEXTURE_PATH + subdirectory + fileName;
    unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        texturesMap.insert(std::pair<std::string, unsigned int>(fileName, textureId));

        std::cout << "Created texture " << fileName << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture " << fileName << std::endl;
    }

    stbi_image_free(data);
}

void cTextureManager::CreateSpriteSheet(std::string spriteSheetName, unsigned int cols, unsigned int rows, bool sym)
{
    if (spriteSheetsMap.count(spriteSheetName)) // texture already created
        return;

    sSpriteSheet newSheet;
    newSheet.numCols = cols;
    newSheet.numRows = rows;
    newSheet.isSymmetrical = sym;

    CreateTexture(spriteSheetName);

    spriteSheetsMap[spriteSheetName] = newSheet;
}

bool cTextureManager::GetTexureId(std::string texture, unsigned int& textureID)
{
    if (texturesMap.find(texture) == texturesMap.end()) // texture doesnt exists
        return false;

    textureID = texturesMap[texture];
    return true;
}

bool cTextureManager::GetSpriteSheet(std::string sheetName, sSpriteSheet& sheet)
{
    if (spriteSheetsMap.find(sheetName) == spriteSheetsMap.end()) // texture doesnt exists
        return false;

    sheet = spriteSheetsMap[sheetName];
    return true;
}

void cTextureManager::SetupTexture(std::string textureToSetup, unsigned int shaderTextureUnit)
{
    if (texturesMap.find(textureToSetup) == texturesMap.end()) // texture doesnt exists
        return;

    unsigned int textureId = texturesMap[textureToSetup];

    std::string shaderVariable = "texture_" + std::to_string(shaderTextureUnit);

    //GLuint textureUnit = 0;			// Texture unit go from 0 to 79
    glActiveTexture(shaderTextureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
    glBindTexture(GL_TEXTURE_2D, textureId);

    cRenderManager::GetInstance()->setInt(shaderVariable, shaderTextureUnit);
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

bool cTextureManager::IsSpriteSymmetric(std::string spriteSheetName)
{
    if (spriteSheetsMap.find(spriteSheetName) == spriteSheetsMap.end())
    {
        std::cout << "Sprite sheet " << spriteSheetName << " doesn't exists" << std::endl;
        return false;
    }
        
    return spriteSheetsMap[spriteSheetName].isSymmetrical;
}
