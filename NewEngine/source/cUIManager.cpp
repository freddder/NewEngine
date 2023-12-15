#include "cUIManager.h"

cUIManager* cUIManager::singleton = NULL;

cUIManager::cUIManager()
{

}

cUIManager::~cUIManager()
{
	while (!canvases.empty())
	{
		delete canvases.top();
		canvases.pop();
	}
}

cUICanvas::~cUICanvas()
{
	for (int i = 0; i < anchoredWidgets.size(); i++)
	{
		delete anchoredWidgets[i].second;
	}
}

void cUICanvas::AddWidget(cUIWidget* newWidget, eAnchors anchor)
{
	anchoredWidgets.push_back(std::pair<eAnchors, cUIWidget*>(anchor, newWidget));
}

void cUIManager::AddCanvas(cUICanvas* newCanvas)
{
	canvases.push(newCanvas);
}

void cUIManager::DrawUI()
{
	const cUICanvas* canvasToDraw = canvases.top();

	for (int i = 0; i < canvasToDraw->anchoredWidgets.size(); i++)
	{
		canvasToDraw->anchoredWidgets[i].second->DrawWidget();
	}
}