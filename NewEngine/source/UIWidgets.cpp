#include "UIWidgets.h"
#include "cCamera.h"

// Should be removed (maybe?)
#include "cRenderManager.h"

cUIWidget::~cUIWidget()
{
	for (int i = 0; i < children.size(); i++)
	{
		delete children[i];
	}
}

void cUIWidget::SetupWidget()
{
	
}

void cUIWidget::AddChild(cUIWidget* newChild)
{
	newChild->parent = this;
	children.push_back(newChild);
}

const float cUIWidget::CalculateHeightPixels()
{
	if (!parent) return heightPercent * cCamera::GetInstance()->SCR_HEIGHT;

	return heightPercent * parent->CalculateHeightPixels();
}

const float cUIWidget::CalculateHeightScreenPercent()
{
	if (!parent) return heightPercent;

	float heightInPixels = parent->CalculateHeightPixels() * heightPercent;

	return heightInPixels / cCamera::GetInstance()->SCR_HEIGHT;
}

const float cUIWidget::CalculateWidthPixels()
{
	return CalculateHeightPixels() / aspectRatio;
}

const float cUIWidget::CalculateWidthScreenPercent()
{
	return CalculateWidthPixels() / cCamera::GetInstance()->SCR_WIDTH;
}

const float cUIWidget::CalculateVerticalTranslate()
{
	float parentVerticalTranslation = 0.f;
	float parentHeightPixels = (float)cCamera::GetInstance()->SCR_HEIGHT;
	if (parent)
	{
		parentVerticalTranslation = parent->CalculateVerticalTranslate();
		parentHeightPixels = parent->CalculateHeightPixels();
	}

	if (anchor == MIDDLE_LEFT || anchor == MIDDLE_MIDDLE || anchor == MIDDLE_RIGHT)	return parentVerticalTranslation;

	float widgetPixelTranslate = parentHeightPixels - CalculateHeightPixels();
	float widgetPercentTranslate = widgetPixelTranslate / (float)cCamera::GetInstance()->SCR_HEIGHT;

	if (anchor == BOTTOM_LEFT || anchor == BOTTOM_MIDDLE || anchor == BOTTOM_RIGHT) widgetPercentTranslate *= -1.f;

	return parentVerticalTranslation + widgetPercentTranslate;
}

const float cUIWidget::CalculateHorizontalTranslate()
{
	float parentHorizontalTranslation = 0.f;
	float parentWidthPixels = (float)cCamera::GetInstance()->SCR_WIDTH;
	if (parent)
	{
		parentHorizontalTranslation = parent->CalculateHorizontalTranslate();
		parentWidthPixels = parent->CalculateWidthPixels();
	}

	if (anchor == TOP_MIDDLE || anchor == MIDDLE_MIDDLE || anchor == BOTTOM_MIDDLE)	return parentHorizontalTranslation;

	float widgetPixelTranslate = parentWidthPixels - CalculateWidthPixels();
	float widgetPercentTranslate = widgetPixelTranslate / (float)cCamera::GetInstance()->SCR_WIDTH;

	if (anchor == TOP_LEFT || anchor == MIDDLE_LEFT || anchor == BOTTOM_LEFT) widgetPercentTranslate *= -1.f;

	return parentHorizontalTranslation + widgetPercentTranslate;
}

void cUIStaticImage::SetupWidget()
{
	cRenderManager::GetInstance()->use("ui");
	cRenderManager::GetInstance()->SetupTexture(textureName);
}
