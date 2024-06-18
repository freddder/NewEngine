#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <set>
#include <map>
#include <memory>
#include "DrawInfo.h"
#include "cRenderModel.h"

const static std::string SHADER_PATH = "assets/shaders/";
const static std::string MODEL_PATH = "assets/models/";
const static std::string TEXTURE_PATH = "assets/textures/";
const static std::string PKM_DATA_PATH = "assets/pokemon/";
const static std::string FONTS_PATH = "assets/fonts/";

const unsigned int SHADOW_WIDTH = 3048, SHADOW_HEIGHT = 3048;
const int FONT_ATLAS_COLS = 10;
const int FONT_ATLAS_ROWS = 9;

class cUIText;

namespace Pokemon
{
    struct sSpeciesData;
    struct sPokemonData;
}

struct sShaderProgram
{
    unsigned int ID;
    std::map<std::string, sModelDrawInfo> modelsLoaded; // stored by file name
    std::map<std::string, unsigned int> uniformLocations;
};

enum eAnimatedModel
{
    OCEAN,  // 0
    FOAM,   // 1
    WAVE,   // 2
    TREE    // 3
};

struct sTexture
{
    unsigned int textureId;
};

struct sSpriteSheet : sTexture
{
public:
    unsigned int numCols;
    unsigned int numRows;
    bool isSymmetrical;
};

struct sFontCharData
{
    glm::ivec2   size;       // Size of glyph
    glm::ivec2   bearing;    // Offset from baseline to left/top of glyph
    unsigned int advance;    // Offset to advance to next glyph
};

struct sFontData
{
    unsigned int textureAtlusId;
    unsigned int glyphSize;
    std::map<char, sFontCharData> characters;
};

enum eRenderMode
{
    MAP,
    BATTLE,
    MENU,
    RENDERMODE_ENUM_COUNT
};

const static char* RenderMode_Strings[] =
{
    "Map",
    "Battle",
    "Menu"
};

class cRenderManager
{
public:
    cRenderManager();
    ~cRenderManager();

    void Startup();
    void Shutdown();

    // Shaders
private:
    std::string currShader;
    std::map<std::string, sShaderProgram> programs;
    void checkCompileErrors(unsigned int shader, std::string type);
public:
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

    // Depth map
private:
    unsigned int depthMapID, depthMapFBO;
public:
    unsigned int GetDepthMapId();

    // Skybox
private:
    unsigned int skyboxVAO, skyboxVBO;
    unsigned int cubemapTextureID;

    // Uniform Buffer Objects
private:
    unsigned int uboMatricesID;
    unsigned int uboFogID;

    // UI quad
private:
    unsigned int uiQuadVAO;
    unsigned int uiQuadVBO;
    unsigned int uiQuadEBO;

    // Models loading
private:
    unsigned int notInstancedOffsetBufferId;
    bool FindModelByName(std::string fileName, std::string programName, sModelDrawInfo& modelInfo);
public:
    bool LoadModel(std::string fileName, std::string programName);

    // Render models
private:
    std::vector< std::shared_ptr<cRenderModel> > mapModels;
    std::vector< std::shared_ptr<cRenderModel> > battleModels; // prob not the best idea
public:
    std::shared_ptr<cRenderModel> CreateRenderModel(bool isBattleModel = false);
    std::shared_ptr<class cSpriteModel> CreateSpriteModel(bool isBattleModel = false);
    std::shared_ptr<class cAnimatedModel> CreateAnimatedModel(eAnimatedModel modelType, bool isBattleModel = false);
    void RemoveModel(std::shared_ptr<cRenderModel> model);

    // Textures
private:
    std::map<std::string, sTexture> mapTextures;
    std::map<std::string, sTexture> battleTextures;
    unsigned int CreateTexture(const std::string fullPath, int& width, int& height);
public:
    void LoadTexture(const std::string fileName, const std::string subdirectory = "", bool isBattleTexture = false);
    void UnloadTextures(bool unloadBattleTexture = false);
    unsigned int CreateCubemap(const std::vector<std::string> faces); // TEMP

private:
    std::map<std::string, sSpriteSheet> sceneSpriteSheets;
    void LoadRoamingPokemonFormSpriteSheet(const int nationalDexId, const std::string formTag = "");
public:
    void LoadSpriteSheet(const std::string spriteSheetName, unsigned int cols, unsigned int rows, bool sym = false, const std::string subdirectory = "");
    void LoadRoamingPokemonSpecieSpriteSheets(const Pokemon::sSpeciesData& specieData);
    void LoadPokemonBattleSpriteSheet(Pokemon::sPokemonData& data); // kinda wanted to make this const but whatever

    void SetupSpriteSheet(const std::string sheetName, const int spriteId, const unsigned int shaderTextureUnit = 0);
    void SetupTexture(const std::string textureToSetup, const unsigned int shaderTextureUnit = 0);

    // Fonts
private:
    std::map<std::string, sFontData> fonts;
    void SetupFont(const std::string fontName);
public:
    void LoadFont(const std::string fontName, const unsigned int glyphSize);
    void CreateTextDataBuffer(cUIText* text);

    // Drawing
private:
    void DrawObject(std::shared_ptr<cRenderModel> model);
    void DrawParticles(class cParticleSpawner* spawner);
    void DrawWidget(class cUIWidget* widget);
    void DrawText(cUIText* textWidget);
    void DrawShadowPass(glm::mat4& outLightSpaceMatrix);
public:
    eRenderMode renderMode = MAP; // TEMP public
    void ChangeRenderMode(eRenderMode newMode);
    void DrawFrame();
};