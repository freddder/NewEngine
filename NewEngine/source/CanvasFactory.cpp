#include "CanvasFactory.h"

#include "Engine.h"
#include "cInputManager.h"
#include "cSceneManager.h"

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

    fightButton = new cUIStaticImage();
    fightButton->anchor = TOP_LEFT;
    fightButton->heightPercent = 0.5f;
    fightButton->aspectRatio = (float)imageHeight / (float)imageWidth;
    fightButton->textureId = LoadUITexture("button.png");
    fightButton->hoveredTextureId = LoadUITexture("button2.png");
    menuBtnContainer->AddChild(fightButton);

    cUIText* fightText = new cUIText();
    fightText->fontName = "Truth And Ideals-Normal.ttf";
    fightText->text = "Fight";
    fightText->heightPercent = 0.3f;
    fightButton->AddChild(fightText);
    Manager::ui.CreateTextDataBuffer(fightText);

    pokemonButton = new cUIStaticImage();
    pokemonButton->anchor = TOP_RIGHT;
    pokemonButton->heightPercent = 0.5f;
    pokemonButton->aspectRatio = (float)imageHeight / (float)imageWidth;
    pokemonButton->textureId = LoadUITexture("button.png");
    pokemonButton->hoveredTextureId = LoadUITexture("button2.png");
    menuBtnContainer->AddChild(pokemonButton);

    cUIText* pkmText = new cUIText();
    pkmText->fontName = "Truth And Ideals-Normal.ttf";
    pkmText->text = "Pokemon";
    pkmText->heightPercent = 0.3f;
    pokemonButton->AddChild(pkmText);
    Manager::ui.CreateTextDataBuffer(pkmText);

    bagButton = new cUIStaticImage();
    bagButton->anchor = BOTTOM_LEFT;
    bagButton->heightPercent = 0.5f;
    bagButton->aspectRatio = (float)imageHeight / (float)imageWidth;
    bagButton->textureId = LoadUITexture("button.png");
    bagButton->hoveredTextureId = LoadUITexture("button2.png");
    menuBtnContainer->AddChild(bagButton);

    cUIText* bagText = new cUIText();
    bagText->fontName = "Truth And Ideals-Normal.ttf";
    bagText->text = "Bag";
    bagText->heightPercent = 0.3f;
    bagButton->AddChild(bagText);
    Manager::ui.CreateTextDataBuffer(bagText);

    runButton = new cUIStaticImage();
    runButton->anchor = BOTTOM_RIGHT;
    runButton->heightPercent = 0.5f;
    runButton->aspectRatio = (float)imageHeight / (float)imageWidth;
    runButton->textureId = LoadUITexture("button.png");
    runButton->hoveredTextureId = LoadUITexture("button2.png");
    menuBtnContainer->AddChild(runButton);

    cUIText* runText = new cUIText();
    runText->fontName = "Truth And Ideals-Normal.ttf";
    runText->text = "Run";
    runText->heightPercent = 0.3f;
    runButton->AddChild(runText);
    Manager::ui.CreateTextDataBuffer(runText);

    defaultFocus = fightButton;

    fightButton->SetMoveFocus(pokemonButton, RIGHT, true);
    fightButton->SetMoveFocus(bagButton, DOWN, true);
    pokemonButton->SetMoveFocus(runButton, DOWN, true);
    bagButton->SetMoveFocus(runButton, RIGHT, true);

    anchoredWidgets.push_back(menuBtnContainer);
}

cBattleCanvas::~cBattleCanvas()
{
}

void cBattleCanvas::ConfirmAction()
{
    if (currFocus == fightButton)
    {
        Manager::scene.ExitEncounter();
    }
    else if (currFocus == pokemonButton)
    {

    }
    else if (currFocus == bagButton)
    {
        Manager::scene.CatchWildPokemon();
    }
    else if (currFocus == runButton)
    {
        Manager::scene.ExitEncounter();
    }
}

void cBattleCanvas::CancelAction()
{
}
