#pragma once
#include <vector>
#include <string>

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
	BOTTOM_RIGHT
};

class cUIWidget
{
public:

	virtual ~cUIWidget();

	bool focusable = false;
	
	float heightPercent = 1.f; // Number from 0 to 1 that represents percentage of vertical space this widget take from parent (or window height if parent is null)
	float aspectRatio = 1.f; // Ratio between widget height and width (height / width)
	eAnchor anchor = MIDDLE_MIDDLE;

	virtual void SetupWidget();

private:
	std::vector<cUIWidget*> children;
	cUIWidget* parent;
public:
	// Do NOT set manually, use AddChild functions
	void AddChild(cUIWidget* newChild);

protected:
	const float CalculateHeightPixels();
	const float CalculateHeightScreenPercent();
	const float CalculateWidthPixels();
	const float CalculateWidthScreenPercent();
	const float CalculateVerticalTranslate();
	const float CalculateHorizontalTranslate();

	friend class cRenderManager;
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