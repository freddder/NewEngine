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
    static cUIManager* singleton;
    cUIManager();
    ~cUIManager();
    cUIManager(const cUIManager& obj) = delete;
public:
    static cUIManager* GetInstance()
    {
        if (singleton == NULL)
        {
            singleton = new cUIManager();
        }

        return singleton;
    }
    static void DestroyInstance()
    {
        if (singleton != NULL)
        {
            delete singleton;
            singleton = NULL;
        }
    }

private:
    std::stack<cUICanvas*> canvases;
public:
    void AddCanvas(cUICanvas* newCanvas);

    friend class cRenderManager;
};