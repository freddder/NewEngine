#pragma once
#include "UIWidgets.h"

#include <stack>
#include <map>

enum eInputType;

class cUICanvas
{
public:
    virtual ~cUICanvas();

protected:
    std::vector<cUIWidget*> anchoredWidgets;

    // Textures
protected:
    std::map<std::string, unsigned int> textures;
public:
    // Load a texture to be used on this canvas and return its id. If no subdirectory specified will look in ui folder
    unsigned int LoadUITexture(const std::string fileName, const std::string subdirectory = "");

protected:
    cUIWidget* defaultFocus;
    cUIWidget* currFocus;
    void ConfirmAction();
    virtual void CancelAction();
    void MoveFocus(cUIWidget* newFocus);

    friend class cUIManager;
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

struct sCharBufferData
{
    float posX, posY, sizeX, sizeY;	// position from origin and screen percent size
    float charId;/*colorR, colorG, colorB,*/  // maybe I will add character highlight color
};

class cUIManager
{
public:
    cUIManager();
    ~cUIManager();

    void Startup();
    void Shutdown();

private:
    std::stack<cUICanvas*> canvases;
public:
    //void AddCanvas(cUICanvas* newCanvas);

    // UI quad
private:
    unsigned int uiQuadVAO;
    unsigned int uiQuadVBO;
    unsigned int uiQuadEBO;
public:
    unsigned int GetUIQuadVAO();

    // Fonts
private:
    std::map<std::string, sFontData> fonts;
public:
    void LoadFont(const std::string fontName, const unsigned int glyphSize);
    void CreateTextDataBuffer(cUIText* text);
    unsigned int GetFontGlyphSize(std::string fontName);
    void SetupFont(const std::string fontName);

    // Functionality
public:
    void ExecuteInputAction(eInputType inputType); // For navigation and functionality

public:
    void DrawUI();
};