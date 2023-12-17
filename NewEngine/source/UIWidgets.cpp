#include "UIWidgets.h"
#include "cCamera.h"

// Should be removed (maybe?)
#include "cRenderManager.h"
#include "cTextureManager.h"

cUIWidget::~cUIWidget()
{
	for (int i = 0; i < children.size(); i++)
	{
		delete children[i];
	}
}

void cUIWidget::DrawWidget()
{
	for (int i = 0; i < children.size(); i++)
	{
		children[i]->DrawWidget();
	}
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

void cUIStaticImage::DrawWidget()
{
	cUIWidget::DrawWidget();

	cRenderManager::GetInstance()->use("debug");
	cTextureManager::GetInstance()->SetupTexture(textureName);

	float widthPercent = CalculateWidthScreenPercent();
	float heightPercent = CalculateHeightScreenPercent();

	cRenderManager::GetInstance()->setFloat("widthPercent", widthPercent);
	cRenderManager::GetInstance()->setFloat("heightPercent", heightPercent);

	float widthTranslate = CalculateHorizontalTranslate();
	float heightTranslate = CalculateVerticalTranslate();

	cRenderManager::GetInstance()->setFloat("widthTranslate", widthTranslate);
	cRenderManager::GetInstance()->setFloat("heightTranslate", heightTranslate);

	glBindVertexArray(cRenderManager::GetInstance()->quadVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
