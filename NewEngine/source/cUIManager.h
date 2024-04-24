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
    static cUIManager* sgtn;
    cUIManager();
    ~cUIManager();
    cUIManager(const cUIManager& obj) = delete;
public:
    static cUIManager* GetInstance()
    {
        if (sgtn == NULL)
        {
            sgtn = new cUIManager();
        }

        return sgtn;
    }
    static void DestroyInstance()
    {
        if (sgtn != NULL)
        {
            delete sgtn;
            sgtn = NULL;
        }
    }

private:
    std::stack<cUICanvas*> canvases;
public:
    void AddCanvas(cUICanvas* newCanvas);

    friend class cRenderManager;
};