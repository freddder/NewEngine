#pragma once
#include "UIElements.h"

#include <stack>

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

    void DrawUI();
};