#include "cUIManager.h"

cUIManager* cUIManager::sgtn = NULL;

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
		delete anchoredWidgets[i];
	}
}

void cUICanvas::AddWidget(cUIWidget* newWidget)
{
	anchoredWidgets.push_back(newWidget);
}

void cUIManager::AddCanvas(cUICanvas* newCanvas)
{
	canvases.push(newCanvas);
}