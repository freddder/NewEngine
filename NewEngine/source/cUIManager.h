#pragma once
#include "UIWidgets.h"

#include <stack>

class cUICanvas
{
public:
    ~cUICanvas();

private:
    std::vector<cUIWidget*> anchoredWidgets;
    cUIWidget* currentFocus;

public:
    void AddWidget(cUIWidget* newWidget);

    //friend class cUIManager;
    friend class cRenderManager;
};

class cUIManager
{
public:
    cUIManager();
    ~cUIManager();

    void Startup();
    void Shutdown();

private:
    std::stack<cUICanvas*> canvases;
public:
    void AddCanvas(cUICanvas* newCanvas);

    friend class cRenderManager;
};