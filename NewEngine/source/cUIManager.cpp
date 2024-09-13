#include "cUIManager.h"

#include <glad/glad.h>
#include <iostream>
#include <sstream>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Engine.h"
#include "cRenderManager.h"
#include "cInputManager.h"

#include "CanvasFactory.h"

const std::string UI_TEXTURE_PATH = "assets/textures/ui/";
const std::string FONTS_PATH = "assets/fonts/";
const int FONT_ATLAS_COLS = 10;
const int FONT_ATLAS_ROWS = 9;

cUIManager::cUIManager()
{
}

cUIManager::~cUIManager()
{
}

void cUIManager::Startup()
{
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

    glGenVertexArrays(1, &uiQuadVAO);
    glGenBuffers(1, &uiQuadVBO);
    glGenBuffers(1, &uiQuadEBO);

    glBindVertexArray(uiQuadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, uiQuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiQuadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndicies), quadIndicies, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    LoadFont("Truth And Ideals-Normal.ttf", 24);

    // Will probably move this to a better place
    cOverworldCanvas* oc = new cOverworldCanvas();
    canvases.push(oc);
}

void cUIManager::Shutdown()
{
	while (!canvases.empty())
	{
		delete canvases.top();
		canvases.pop();
	}

    for (std::map<std::string, sFontData>::iterator it = fonts.begin(); it != fonts.end(); it++)
    {
        glDeleteBuffers(1, &it->second.textureAtlusId);
    }

    glDeleteVertexArrays(1, &uiQuadVAO);
}

void cUIManager::AddCanvas(cUICanvas* newCanvas)
{
    canvases.push(newCanvas);
}

void cUIManager::RemoveCanvas()
{
    cUICanvas* canvasToRemove = canvases.top();
    canvases.pop();
    delete canvasToRemove;
}

cUICanvas::~cUICanvas()
{
	for (int i = 0; i < anchoredWidgets.size(); i++)
	{
		delete anchoredWidgets[i];
	}

    for (std::map<std::string, unsigned int>::iterator it = textures.begin(); it != textures.end(); it++)
    {
        glDeleteBuffers(1, &it->second);
    }
}

unsigned int cUICanvas::LoadUITexture(const std::string fileName, const std::string subdirectory)
{
    if (fileName == "") return 0;

    if (textures.find(fileName) != textures.end()) return textures[fileName]; // texture already loaded

    std::string fullPath = "";
    if (subdirectory == "") fullPath += UI_TEXTURE_PATH + fileName;
    else fullPath += subdirectory + fileName;

    int width, height;
    unsigned int textureId = Manager::render.CreateTexture(fullPath, width, height);

    if (textureId != 0)
    {
        textures.insert(std::pair<std::string, unsigned int>(fileName, textureId));
    }

    return textureId;
}

void cUICanvas::ConfirmAction()
{
    if (currFocus)
        currFocus->ConfirmAction();
}

void cUICanvas::CancelAction()
{
}

void cUICanvas::MoveFocus(cUIWidget* newFocus)
{
    if (!newFocus) return;

    currFocus->LeaveFocus();
    currFocus = newFocus;
    currFocus->EnterFocus();
}

void cUICanvas::ResetFocus()
{
    if (currFocus)
    {
        currFocus->LeaveFocus();
        currFocus = nullptr;
    }
}

unsigned int cUIManager::GetUIQuadVAO()
{
    return uiQuadVAO;
}

void cUIManager::LoadFont(const std::string fontName, const unsigned int glyphSize)
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

// Make sure to call this after setting this widget's parent
void cUIManager::CreateTextDataBuffer(cUIText* text)
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

unsigned int cUIManager::GetFontGlyphSize(std::string fontName)
{
    return fonts[fontName].glyphSize;
}

void cUIManager::SetupFont(const std::string fontName)
{
    if (fonts.find(fontName) == fonts.end()) return; // font doesn't exists

    unsigned int textureId = fonts[fontName].textureAtlusId;

    glActiveTexture(GL_TEXTURE0);	// GL_TEXTURE0 = 33984
    glBindTexture(GL_TEXTURE_2D, textureId);

    std::string shaderVariable = "texture_0";
    Manager::render.setInt(shaderVariable, 0);

    Manager::render.setInt("atlasRowsNum", FONT_ATLAS_ROWS);
    Manager::render.setInt("atlasColsNum", FONT_ATLAS_COLS);
    Manager::render.setInt("glyphSize", fonts[fontName].glyphSize);
}

void cUIManager::ExecuteInputAction(eInputType inputType)
{
    cUICanvas* currCanvas = canvases.top();

    if (inputType == IT_CONFIRM)
    {
        currCanvas->ConfirmAction();
        return;
    }
    else if (inputType == IT_CANCEL)
    {
        currCanvas->CancelAction();
        return;
    }

    if (!currCanvas->currFocus)
    {
        currCanvas->currFocus = currCanvas->defaultFocus;
        currCanvas->currFocus->EnterFocus();
        return;
    }

    switch (inputType)
    {
    case IT_UP:
        currCanvas->MoveFocus(currCanvas->currFocus->focusUp);
        break;
    case IT_DOWN:
        currCanvas->MoveFocus(currCanvas->currFocus->focusDown);
        break;
    case IT_LEFT:
        currCanvas->MoveFocus(currCanvas->currFocus->focusLeft);
        break;
    case IT_RIGHT:
        currCanvas->MoveFocus(currCanvas->currFocus->focusRight);
        break;
    }
}
void cUIManager::DrawUI()
{
	const cUICanvas* canvasToDraw = canvases.top();
	for (int i = 0; i < canvasToDraw->anchoredWidgets.size(); i++)
	{
	    canvasToDraw->anchoredWidgets[i]->Draw();
	}
}
