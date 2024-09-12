#include "CanvasFactory.h"

#include "Engine.h"
#include "cInputManager.h"
#include "cAnimation.h"

cOverworldCanvas::cOverworldCanvas()
{
    cUIWidget* menuBtnContainer = new cUIWidget();
    menuBtnContainer->anchor = MIDDLE_RIGHT;
    menuBtnContainer->aspectRatio = 230.f / 126.f;
    menuBtnContainer->heightPercent = 3.f / 4.f; // change this for size

    cUIWidget* topBtnContainer = new cUIWidget();
    topBtnContainer->anchor = TOP_MIDDLE;
    topBtnContainer->aspectRatio = 92.f / 126.f;
    topBtnContainer->heightPercent = 2.f / 5.f;

    cMenuButtonWidget* menuBtn1 = new cMenuButtonWidget(this, "Pokedex", "");
    menuBtn1->anchor = TOP_MIDDLE;
    menuBtn1->heightPercent = 0.5f;
    topBtnContainer->AddChild(menuBtn1);

    cMenuButtonWidget* menuBtn2 = new cMenuButtonWidget(this, "Pokemon", "");
    menuBtn2->anchor = BOTTOM_MIDDLE;
    menuBtn2->heightPercent = 0.5f;
    topBtnContainer->AddChild(menuBtn2);

    cMenuButtonWidget* menuBtn3 = new cMenuButtonWidget(this, "Bag", "");
    menuBtn3->anchor = MIDDLE_MIDDLE;
    menuBtn3->heightPercent = 1.f / 5.f;
    menuBtnContainer->AddChild(menuBtn3);

    cUIWidget* botBtnContainer = new cUIWidget();
    botBtnContainer->anchor = BOTTOM_MIDDLE;
    botBtnContainer->aspectRatio = 92.f / 126.f;
    botBtnContainer->heightPercent = 2.f / 5.f;

    cMenuButtonWidget* menuBtn4 = new cMenuButtonWidget(this, "Options", "");
    menuBtn4->anchor = TOP_MIDDLE;
    menuBtn4->heightPercent = 0.5f;
    botBtnContainer->AddChild(menuBtn4);

    cMenuButtonWidget* menuBtn5 = new cMenuButtonWidget(this, "Save", "");
    menuBtn5->anchor = BOTTOM_MIDDLE;
    menuBtn5->heightPercent = 0.5f;
    botBtnContainer->AddChild(menuBtn5);

    menuBtnContainer->AddChild(topBtnContainer);
    menuBtnContainer->AddChild(botBtnContainer);

    menuBtn1->SetMoveFocus(menuBtn2, DOWN, true);
    menuBtn2->SetMoveFocus(menuBtn3, DOWN, true);
    menuBtn3->SetMoveFocus(menuBtn4, DOWN, true);
    menuBtn4->SetMoveFocus(menuBtn5, DOWN, true);

    anchoredWidgets.push_back(menuBtnContainer);
    defaultFocus = menuBtn1;
}

cOverworldCanvas::~cOverworldCanvas()
{
}

void cOverworldCanvas::CancelAction()
{
    ResetFocus();
	Manager::input.ChangeInputState(OVERWORLD_MOVEMENT);
}

cMenuButtonWidget::cMenuButtonWidget(cUICanvas* canvas, std::string text, std::string iconFileName)
{
    textureId = canvas->LoadUITexture("PartyMemberButtonBackground.png");
    hoveredTextureId = canvas->LoadUITexture("PartyMemberButtonBackground-Hovered.png");
    aspectRatio = 0.365f;

    cUIStaticImage* icon = new cUIStaticImage();
    icon->anchor = MIDDLE_LEFT;
    icon->aspectRatio = 3.f / 4.f;
    icon->heightPercent = 3.f / 4.f;
    icon->textureId = canvas->LoadUITexture("445_ico_s.png", "assets/pokemon/0445/");
    AddChild(icon);

    cUIText* textWidget = new cUIText();
    textWidget->fontName = "Truth And Ideals-Normal.ttf";
    textWidget->text = text;
    textWidget->heightPercent = 0.3f;
    AddChild(textWidget);
    Manager::ui.CreateTextDataBuffer(textWidget);
}

cMenuButtonWidget::~cMenuButtonWidget()
{
}
