#pragma once
#include <vector>
#include <string>

class cUIWidget
{
public:

	virtual ~cUIWidget();

	bool focusable = false;
	
	float heightPercent = 1.f; // Number from 0 to 1 that represents percentage of vertical space this widget take from parent (or window height if parent is null)
	float aspectRatio = 1.f; // Ratio between widget height and width (height / width)

	virtual void DrawWidget();

private:
	std::vector<cUIWidget*> children;
public:
	// Do NOT set manually, use CreateChild functions
	cUIWidget* parent;
	void AddChild(cUIWidget* newChild);

protected:
	float CalculateHeightPixels();
	float CalculateHeightScreenPercent();
	float CalculateWidthPixels();
	float CalculateWidthScreenPercent();
};

class cUIStaticImage : public cUIWidget
{
	~cUIStaticImage() {}
public:
	std::string textureName;
	virtual void DrawWidget();
};

class cUIAnimatedSprite : public cUIWidget
{
public:
	std::string spriteTextureName;
	virtual void DrawWidget();
};