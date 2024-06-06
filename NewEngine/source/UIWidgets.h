#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

enum eAnchor
{
	TOP_LEFT,
	TOP_MIDDLE,
	TOP_RIGHT,
	MIDDLE_LEFT,
	MIDDLE_MIDDLE,
	MIDDLE_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_MIDDLE,
	BOTTOM_RIGHT,
	ANCHOR_ENUM_COUNT
};

const static char* Anchors_Strings[] =
{
	"Top Left",
	"Top Middle",
	"Top Right",
	"Middle Left",
	"Middle Middle",
	"Middle Right",
	"Bottom Left",
	"Bottom Middle",
	"Bottom Right"
};

class cUIWidget
{
public:

	virtual ~cUIWidget();

	bool focusable = false;
	
	// Number from 0 to 1 that represents percentage of vertical space this widget take from parent (or window height if parent is null)
	float heightPercent = 1.f;
	// Ratio between widget height and width (height / width)
	float aspectRatio = 1.f;
	eAnchor anchor = MIDDLE_MIDDLE;

	virtual void SetupWidget();
	virtual bool ShouldUseTextRenderer() { return false; }

private:
	std::vector<cUIWidget*> children;
	cUIWidget* parent;
public:
	void AddChild(cUIWidget* newChild);

protected:
	const float CalculateHeightPixels();
	const float CalculateHeightScreenPercent();
	const float CalculateWidthPixels();
	const float CalculateWidthScreenPercent();

	const float CalculateVerticalTranslate();
	const float CalculateHorizontalTranslate();

	friend class cRenderManager;
	friend class cUIText;
};

class cUIStaticImage : public cUIWidget
{
	~cUIStaticImage() {}
public:
	std::string textureName;
	virtual void SetupWidget();
};

class cUIAnimatedSprite : public cUIWidget
{
public:
	std::string spriteTextureName;
	virtual void SetupWidget();
};

struct sCharBufferData
{
	int posX, posY, sizeX, sizeY;	// position from origin and screen percent size
	int charId;
};

class cUIText : public cUIWidget
{
public:
	cUIText();
	~cUIText();

	//unsigned int bufferDataId;
	std::vector<sCharBufferData> data;
	unsigned int drawCharCount = 0;
	std::string fontName;
	std::string text;
	glm::vec3 color;
	float textSizePercent = 1.f; // Of this widget

	virtual bool ShouldUseTextRenderer() { return true; }
};