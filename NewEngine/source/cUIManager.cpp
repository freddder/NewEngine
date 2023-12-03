#include "cUIManager.h"

cUIManager* cUIManager::singleton = NULL;

cUIManager::cUIManager()
{

}

cUIManager::~cUIManager()
{

}

void cUICanvas::AddWidget(const cUIWidget& newWidget, eAnchors anchor)
{
	anchoredWidgets.push_back(std::pair<eAnchors, cUIWidget>(anchor, newWidget));
}

void cUIManager::AddCanvas(const cUICanvas& newCanvas)
{
	canvases.push(newCanvas);
}

void cUIManager::DrawUI()
{
	cUICanvas& canvasToDraw = canvases.top();

	for (int i = 0; i < canvasToDraw.anchoredWidgets.size(); i++)
	{
		canvasToDraw.anchoredWidgets[i].second.DrawWidget();
	}
}


