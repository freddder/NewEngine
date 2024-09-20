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

enum eDirection;

class cUIWidget
{
public:

	virtual ~cUIWidget();
	
	// Number from 0 to 1 that represents percentage of vertical space this widget take from parent (or window height if parent is null)
	float heightPercent = 1.f;
	// Ratio between widget height and width (height / width)
	float aspectRatio = 1.f;
	// Percent of parent to translate verticaly (positive up and negative down)
	float verticalTranslate = 0.f;
	// Percent of parent to translate herizontaly (positive right and negative left)
	float horizontalTranslate = 0.f;
	eAnchor anchor = MIDDLE_MIDDLE;

	bool isHidden = false;
	virtual void Draw();

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

protected:
	bool isFocused = false;
	cUIWidget* focusUp;
	cUIWidget* focusDown;
	cUIWidget* focusLeft;
	cUIWidget* focusRight;
	virtual void EnterFocus();
	virtual void LeaveFocus();
	virtual void ConfirmAction();
public:
	void SetMoveFocus(cUIWidget* to, eDirection dir, bool isViceVersa);

	friend class cUIManager;
	friend class cUICanvas;
};

class cUIImage : public cUIWidget
{
public:
	virtual ~cUIImage();

	unsigned int textureId;
	unsigned int hoveredTextureId;

	bool useScreenSpace = false;
	glm::vec2 screenSpaceRatio = glm::vec2(1.f);
	glm::vec2 translate = glm::vec2(0.f);

	glm::vec3 colorFilter = glm::vec3(1.f);

	virtual void Draw();
};

class cUIAnimatedSprite : public cUIWidget
{
public:
	std::string spriteTextureName;
};

class cUIText : public cUIWidget
{
public:
	cUIText();
	virtual ~cUIText();

	unsigned int dataBufferId = 0;
	size_t drawCharCount = 0;
	std::string fontName;
	std::string text;
	glm::vec3 color = glm::vec3(1.f);
	float textSizePercent = 1.f; // Of this widget

	virtual void Draw();
};