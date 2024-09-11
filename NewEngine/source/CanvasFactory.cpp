#include "CanvasFactory.h"

#include "Engine.h"
#include "cInputManager.h"
#include "cAnimation.h"

cOverworldCanvas::cOverworldCanvas()
{
    cUIStaticImage* button = new cUIStaticImage();
    button->anchor = MIDDLE_LEFT;
    button->textureId = LoadUITexture("PartyMemberButtonBackground.png");
    button->hoveredTextureId = LoadUITexture("PartyMemberButtonBackground-Hovered.png");
    button->aspectRatio = 0.365f;
    button->heightPercent = 1.f / 9.f;

    cUIStaticImage* sprite = new cUIStaticImage();
    sprite->anchor = MIDDLE_LEFT;
    sprite->aspectRatio = 3.f / 4.f;
    sprite->heightPercent = 3.f / 4.f;
    sprite->textureId = LoadUITexture("445_ico_s.png", "assets/pokemon/0445/");
    button->AddChild(sprite);

    cUIText* textWidget = new cUIText();
    textWidget->fontName = "Truth And Ideals-Normal.ttf";
    textWidget->text = "Thatguy";
    textWidget->color = glm::vec3(1.f);
    textWidget->heightPercent = 0.3f;
    button->AddChild(textWidget);
    Manager::ui.CreateTextDataBuffer(textWidget);

    cUIStaticImage* button2 = new cUIStaticImage();
    button2->anchor = TOP_LEFT;
    button2->textureId = LoadUITexture("PartyMemberButtonBackground.png");
    button2->hoveredTextureId = LoadUITexture("PartyMemberButtonBackground-Hovered.png");
    button2->aspectRatio = 0.365f;
    button2->heightPercent = 1.f / 9.f;

    cUIStaticImage* sprite2 = new cUIStaticImage();
    sprite2->anchor = MIDDLE_LEFT;
    sprite2->aspectRatio = 3.f / 4.f;
    sprite2->heightPercent = 3.f / 4.f;
    sprite2->textureId = LoadUITexture("445_ico_s.png", "assets/pokemon/0445/");
    button2->AddChild(sprite2);

    cUIText* textWidget2 = new cUIText();
    textWidget2->fontName = "Truth And Ideals-Normal.ttf";
    textWidget2->text = "Thatotherguy";
    textWidget2->color = glm::vec3(1.f);
    textWidget2->heightPercent = 0.3f;
    button2->AddChild(textWidget2);
    Manager::ui.CreateTextDataBuffer(textWidget2);

    button2->SetMoveFocus(button, DOWN, true);

    anchoredWidgets.push_back(button);
    anchoredWidgets.push_back(button2);
    defaultFocus = button;
}

cOverworldCanvas::~cOverworldCanvas()
{
}

void cOverworldCanvas::CancelAction()
{
	//MoveFocus(defaultFocus);
    ResetFocus();
	Manager::input.ChangeInputState(OVERWORLD_MOVEMENT);
}
