#include "UIWidgets.h"

#include <glad/glad.h>

#include "Engine.h"
#include "cUIManager.h"
#include "cCameraManager.h"
#include "cRenderManager.h"

#include "cAnimation.h"

cUIWidget::~cUIWidget()
{
	for (int i = 0; i < children.size(); i++)
	{
		delete children[i];
	}
}

void cUIWidget::Draw()
{
	if (isHidden) return;

	for (int i = 0; i < children.size(); i++)
	{
		children[i]->Draw();
	}
}

void cUIWidget::AddChild(cUIWidget* newChild)
{
	newChild->parent = this;
	children.push_back(newChild);
}

const float cUIWidget::CalculateHeightPixels()
{
	if (!parent) return heightPercent * Manager::camera.SCR_HEIGHT;

	return heightPercent * parent->CalculateHeightPixels();
}

const float cUIWidget::CalculateHeightScreenPercent()
{
	if (!parent) return heightPercent;

	float heightInPixels = parent->CalculateHeightPixels() * heightPercent;
	return heightInPixels / Manager::camera.SCR_HEIGHT;
}

const float cUIWidget::CalculateWidthPixels()
{
	return CalculateHeightPixels() / aspectRatio;
}

const float cUIWidget::CalculateWidthScreenPercent()
{
	return CalculateWidthPixels() / Manager::camera.SCR_WIDTH;
}

const float cUIWidget::CalculateVerticalTranslate()
{
	float parentVerticalTranslation = 0.f;
	float parentHeightPixels = (float)Manager::camera.SCR_HEIGHT;
	if (parent)
	{
		parentVerticalTranslation = parent->CalculateVerticalTranslate();
		parentHeightPixels = parent->CalculateHeightPixels();
	}

	if (anchor == MIDDLE_LEFT || anchor == MIDDLE_MIDDLE || anchor == MIDDLE_RIGHT)	return parentVerticalTranslation;

	float widgetPixelTranslate = parentHeightPixels - CalculateHeightPixels();
	float widgetPercentTranslate = widgetPixelTranslate / (float)Manager::camera.SCR_HEIGHT;

	if (anchor == BOTTOM_LEFT || anchor == BOTTOM_MIDDLE || anchor == BOTTOM_RIGHT) widgetPercentTranslate *= -1.f;

	return parentVerticalTranslation + widgetPercentTranslate;
}

const float cUIWidget::CalculateHorizontalTranslate()
{
	float parentHorizontalTranslation = 0.f;
	float parentWidthPixels = (float)Manager::camera.SCR_WIDTH;
	if (parent)
	{
		parentHorizontalTranslation = parent->CalculateHorizontalTranslate();
		parentWidthPixels = parent->CalculateWidthPixels();
	}

	if (anchor == TOP_MIDDLE || anchor == MIDDLE_MIDDLE || anchor == BOTTOM_MIDDLE)	return parentHorizontalTranslation;

	float widgetPixelTranslate = parentWidthPixels - CalculateWidthPixels();
	float widgetPercentTranslate = widgetPixelTranslate / (float)Manager::camera.SCR_WIDTH;

	if (anchor == TOP_LEFT || anchor == MIDDLE_LEFT || anchor == BOTTOM_LEFT) widgetPercentTranslate *= -1.f;

	return parentHorizontalTranslation + widgetPercentTranslate;
}

void cUIWidget::SetMoveFocus(cUIWidget* to, eDirection dir, bool isViceVersa)
{
	if (!to) return;

	switch (dir)
	{
	case UP:
		focusUp = to;
		break;
	case DOWN:
		focusDown = to;
		break;
	case LEFT:
		focusLeft = to;
		break;
	case RIGHT:
		focusRight = to;
		break;
	default:
		break;
	}

	if (isViceVersa)
	{
		switch (dir)
		{
		case UP:
			to->focusDown = this;
			break;
		case DOWN:
			to->focusUp = this;
			break;
		case LEFT:
			to->focusRight = this;
			break;
		case RIGHT:
			to->focusLeft = this;
			break;
		default:
			break;
		}
	}
}

void cUIWidget::EnterFocus()
{
	for (int i = 0; i < children.size(); i++)
	{
		children[i]->EnterFocus();
	}
}

void cUIWidget::LeaveFocus()
{
	for (int i = 0; i < children.size(); i++)
	{
		children[i]->LeaveFocus();
	}
}

void cUIWidget::ConfirmAction()
{
}

void cUIStaticImage::Draw()
{
	cUIWidget::Draw();

	if (textureId == 0) return;

	Manager::render.use("ui");
	
	unsigned int shaderTextureUnit = 0;
	glActiveTexture(shaderTextureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
	glBindTexture(GL_TEXTURE_2D, textureId);
	std::string shaderVariable = "texture_" + std::to_string(shaderTextureUnit);
	Manager::render.setInt(shaderVariable, shaderTextureUnit);

	// OPTMIZATION: calculate these values once and store them and/or make them into a matrix
	float widthPercent = CalculateWidthScreenPercent();
	float heightPercent = CalculateHeightScreenPercent();
	Manager::render.setFloat("widthPercent", widthPercent);
	Manager::render.setFloat("heightPercent", heightPercent);

	float widthTranslate = CalculateHorizontalTranslate();
	float heightTranslate = CalculateVerticalTranslate();
	Manager::render.setFloat("widthTranslate", widthTranslate);
	Manager::render.setFloat("heightTranslate", heightTranslate);

	glBindVertexArray(Manager::ui.GetUIQuadVAO());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

cUIText::cUIText()
{
}

cUIText::~cUIText()
{
	if (dataBufferId != 0)
	{
		glDeleteBuffers(1, &dataBufferId);
	}
}

void cUIText::Draw()
{
	unsigned int scrWidth = Manager::camera.SCR_WIDTH;
	unsigned int scrHeight = Manager::camera.SCR_HEIGHT;
	float horizontalTranslation = CalculateHorizontalTranslate();
	float verticalTranslation = CalculateVerticalTranslate();
	float widthPercent = CalculateWidthScreenPercent();
	float heightPercent = CalculateHeightScreenPercent();

	float pixelGlyphRatio = heightPercent * scrHeight * textSizePercent / (float)Manager::ui.GetFontGlyphSize(fontName);

	float finalHorizontalTranslation = horizontalTranslation - widthPercent;
	float finalVerticalTranslation = verticalTranslation + heightPercent;
	glm::vec2 origin = glm::vec2(finalHorizontalTranslation, finalVerticalTranslation);

	Manager::render.use("text");
	Manager::ui.SetupFont(fontName);
	Manager::render.setVec3("color", color);

	Manager::render.setVec2("originOffset", origin);
	Manager::render.setFloat("glyphPixelRatio", pixelGlyphRatio);
	Manager::render.setInt("screenWidth", scrWidth);
	Manager::render.setInt("screenHeight", scrHeight);

	glBindVertexArray(Manager::ui.GetUIQuadVAO());

	// Setup buffer data as vertex atribute
	// (ideally I would want this to be set on VAO creation, but I guess the data needs to be setup before hand... so here it goes)
	glBindBuffer(GL_ARRAY_BUFFER, dataBufferId);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribDivisor(2, 1);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(4 * sizeof(float)));
	glVertexAttribDivisor(3, 1);

	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0, drawCharCount);

	glBindVertexArray(0);
}
