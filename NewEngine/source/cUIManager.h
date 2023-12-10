#pragma once
#include "UIWidgets.h"

#include <stack>

class cUICanvas
{
public:
    enum eAnchors
    {
        TOP_LEFT,
        TOP_MIDDLE,
        TOP_RIGHT,
        MIDLLE_LEFT,
        MIDDLE_MIDDLE,
        MIDDLE_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_MIDDLE,
        BOTTOM_RIGHT
    };
private:
    std::vector<std::pair<eAnchors, cUIWidget>> anchoredWidgets;
    cUIWidget* currentFocus;

public:
    void AddWidget(const cUIWidget& newWidget, eAnchors anchor = MIDDLE_MIDDLE);

    friend class cUIManager;
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
    std::stack<cUICanvas> canvases;
public:
    void AddCanvas(const cUICanvas& newCanvas);

public:
    void DrawUI();
};