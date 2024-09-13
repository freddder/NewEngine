#include "CanvasFactory.h"

#include "Engine.h"
#include "cInputManager.h"
#include "cAnimation.h"

cMenuButtonWidget::cMenuButtonWidget(cUICanvas* canvas, std::string text, std::string iconFileName)
{
    textureId = canvas->LoadUITexture("panel.png");
    hoveredTextureId = canvas->LoadUITexture("panel2.png");
    aspectRatio = 82.f / 250.f;

    cUIStaticImage* icon = new cUIStaticImage();
    icon->anchor = MIDDLE_LEFT;
    icon->aspectRatio = 1.f;;
    icon->heightPercent = 3.f / 4.f;
    icon->textureId = canvas->LoadUITexture(iconFileName);
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

cOverworldCanvas::cOverworldCanvas()
{
    int imageWidth = 250;
    int imageHeight = 82;

    cUIWidget* menuBtnContainer = new cUIWidget();    
    menuBtnContainer->anchor = MIDDLE_RIGHT;
    menuBtnContainer->aspectRatio = (float)imageHeight * 5 / (float)imageWidth;
    menuBtnContainer->heightPercent = 3.f / 4.f; // change this for size

    cUIWidget* topBtnContainer = new cUIWidget();
    topBtnContainer->anchor = TOP_MIDDLE;
    topBtnContainer->aspectRatio = (float)imageHeight * 2 / (float)imageWidth;
    topBtnContainer->heightPercent = 2.f / 5.f;

    cMenuButtonWidget* menuBtn1 = new cMenuButtonWidget(this, "Pokedex", "pokedex.png");
    menuBtn1->anchor = TOP_MIDDLE;
    menuBtn1->heightPercent = 0.5f;
    topBtnContainer->AddChild(menuBtn1);

    cMenuButtonWidget* menuBtn2 = new cMenuButtonWidget(this, "Pokemon", "party.png");
    menuBtn2->anchor = BOTTOM_MIDDLE;
    menuBtn2->heightPercent = 0.5f;
    topBtnContainer->AddChild(menuBtn2);

    cMenuButtonWidget* menuBtn3 = new cMenuButtonWidget(this, "Bag", "bag.png");
    menuBtn3->anchor = MIDDLE_MIDDLE;
    menuBtn3->heightPercent = 1.f / 5.f;
    menuBtnContainer->AddChild(menuBtn3);

    cUIWidget* botBtnContainer = new cUIWidget();
    botBtnContainer->anchor = BOTTOM_MIDDLE;
    botBtnContainer->aspectRatio = 92.f / 126.f;
    botBtnContainer->heightPercent = 2.f / 5.f;

    cMenuButtonWidget* menuBtn4 = new cMenuButtonWidget(this, "Options", "options.png");
    menuBtn4->anchor = TOP_MIDDLE;
    menuBtn4->heightPercent = 0.5f;
    botBtnContainer->AddChild(menuBtn4);

    cMenuButtonWidget* menuBtn5 = new cMenuButtonWidget(this, "Save", "save.png");
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

cBattleCanvas::cBattleCanvas()
{
    int imageHeight = 46;
    int imageWidth = 126;

    cUIWidget* menuBtnContainer = new cUIWidget();
    menuBtnContainer->anchor = BOTTOM_RIGHT;
    menuBtnContainer->aspectRatio = (float)imageHeight / (float)imageWidth;
    menuBtnContainer->heightPercent = 1.f / 4.f; // change this for size

    cUIStaticImage* fightBtn = new cUIStaticImage();
    fightBtn->anchor = TOP_LEFT;
    fightBtn->heightPercent = 0.5f;
    fightBtn->aspectRatio = (float)imageHeight / (float)imageWidth;
    fightBtn->textureId = LoadUITexture("button.png");
    fightBtn->hoveredTextureId = LoadUITexture("button2.png");
    menuBtnContainer->AddChild(fightBtn);

    cUIText* fightText = new cUIText();
    fightText->fontName = "Truth And Ideals-Normal.ttf";
    fightText->text = "Fight";
    fightText->heightPercent = 0.3f;
    fightBtn->AddChild(fightText);
    Manager::ui.CreateTextDataBuffer(fightText);

    cUIStaticImage* pkmBtn = new cUIStaticImage();
    pkmBtn->anchor = TOP_RIGHT;
    pkmBtn->heightPercent = 0.5f;
    pkmBtn->aspectRatio = (float)imageHeight / (float)imageWidth;
    pkmBtn->textureId = LoadUITexture("button.png");
    pkmBtn->hoveredTextureId = LoadUITexture("button2.png");
    menuBtnContainer->AddChild(pkmBtn);

    cUIText* pkmText = new cUIText();
    pkmText->fontName = "Truth And Ideals-Normal.ttf";
    pkmText->text = "Pokemon";
    pkmText->heightPercent = 0.3f;
    pkmBtn->AddChild(pkmText);
    Manager::ui.CreateTextDataBuffer(pkmText);

    cUIStaticImage* bagBtn = new cUIStaticImage();
    bagBtn->anchor = BOTTOM_LEFT;
    bagBtn->heightPercent = 0.5f;
    bagBtn->aspectRatio = (float)imageHeight / (float)imageWidth;
    bagBtn->textureId = LoadUITexture("button.png");
    bagBtn->hoveredTextureId = LoadUITexture("button2.png");
    menuBtnContainer->AddChild(bagBtn);

    cUIText* bagText = new cUIText();
    bagText->fontName = "Truth And Ideals-Normal.ttf";
    bagText->text = "Bag";
    bagText->heightPercent = 0.3f;
    bagBtn->AddChild(bagText);
    Manager::ui.CreateTextDataBuffer(bagText);

    cUIStaticImage* runBtn = new cUIStaticImage();
    runBtn->anchor = BOTTOM_RIGHT;
    runBtn->heightPercent = 0.5f;
    runBtn->aspectRatio = (float)imageHeight / (float)imageWidth;
    runBtn->textureId = LoadUITexture("button.png");
    runBtn->hoveredTextureId = LoadUITexture("button2.png");
    menuBtnContainer->AddChild(runBtn);

    cUIText* runText = new cUIText();
    runText->fontName = "Truth And Ideals-Normal.ttf";
    runText->text = "Run";
    runText->heightPercent = 0.3f;
    runBtn->AddChild(runText);
    Manager::ui.CreateTextDataBuffer(runText);

    defaultFocus = fightBtn;

    fightBtn->SetMoveFocus(pkmBtn, RIGHT, true);
    fightBtn->SetMoveFocus(bagBtn, DOWN, true);
    pkmBtn->SetMoveFocus(runBtn, DOWN, true);
    bagBtn->SetMoveFocus(runBtn, RIGHT, true);

    anchoredWidgets.push_back(menuBtnContainer);
}

cBattleCanvas::~cBattleCanvas()
{
}
