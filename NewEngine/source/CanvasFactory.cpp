#include "CanvasFactory.h"

#include "Engine.h"
#include "cInputManager.h"
#include "cSceneManager.h"
#include "cCameraManager.h"
#include "cAnimationManager.h"

#include "cAnimation.h"

#include "Player.h"

cMenuButtonWidget::cMenuButtonWidget(cUICanvas* canvas, std::string text, std::string iconFileName)
{
    textureId = canvas->LoadUITexture("panel.png");
    hoveredTextureId = canvas->LoadUITexture("panel2.png");
    aspectRatio = 82.f / 250.f;

    cUIImage* icon = new cUIImage();
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

cOverworldCanvas::cOverworldCanvas()
{
    int imageWidth = 250;
    int imageHeight = 82;

    cUIWidget* menuBtnContainer = new cUIWidget();    
    menuBtnContainer->anchor = BOTTOM_RIGHT;
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

void cOverworldCanvas::ConfirmAction()
{
    Engine::currGameMode = eGameMode::MENU;
    Manager::ui.AddCanvas(new cPartyCanvas());
}

void cOverworldCanvas::CancelAction()
{
    ResetFocus();
	Manager::input.ChangeInputState(OVERWORLD_MOVEMENT);
}

cHealthBar::cHealthBar(cUICanvas* canvas)
{
    aspectRatio = 0.1f;
    textureId = canvas->LoadUITexture("HealthBarBackground.png");

    cUIWidget* fillable = new cUIWidget();
    fillable->aspectRatio = 0.0625f;
    fillable->heightPercent = 3.f / 5.f;
    fillable->anchor = MIDDLE_MIDDLE;

    hbContent = new cUIImage();
    hbContent->aspectRatio = 0.0625f; //* 2.f; // this being half (just inverse the health percentage lol)
    hbContent->anchor = MIDDLE_LEFT;
    hbContent->textureId = canvas->LoadUITexture("HealthBar.png");
    hbContent->colorFilter = glm::vec3(0.f, 1.f, 0.f);
    fillable->AddChild(hbContent);

    // all I have to do is change the aspect ratio (idk the numbers tho)
    AddChild(fillable);
}

void cHealthBar::UpdateHealthBar(float healthPercent)
{
    float percentToUse = glm::clamp(healthPercent, 0.f, 1.f);
    
    hbContent->aspectRatio = 0.0625f * (1.f / healthPercent);

    if (healthPercent < 0.25f) 
        hbContent->colorFilter = glm::vec3(1.f, 0.f, 0.f); // red
    else if (healthPercent < 0.5f)
        hbContent->colorFilter = glm::vec3(1.f, 1.f, 0.f); // yellow
    else 
        hbContent->colorFilter = glm::vec3(0.f, 1.f, 0.f); // green
}

cPlayerBattleInfo::cPlayerBattleInfo(cUICanvas* canvas)
{
    aspectRatio = 28.f / 113.f;

    cUIImage* bg = new cUIImage();
    bg->aspectRatio = 13.f / 113.f;
    bg->heightPercent = 13.f / 28.f;
    bg->verticalTranslate = 4.f / 28.f;
    bg->anchor = BOTTOM_MIDDLE;
    bg->textureId = canvas->LoadUITexture("BattlePlayerBackground.png");

    playeHb = new cHealthBar(canvas);
    playeHb->heightPercent = 5.f / 28.f;
    playeHb->anchor = TOP_RIGHT;
    playeHb->verticalTranslate = -11.f / 28.f;
    playeHb->horizontalTranslate = -14.f / 113.f;

    cUIImage* hpIcon = new cUIImage();
    hpIcon->aspectRatio = 7.f / 16.f;
    hpIcon->heightPercent = 7.f / 28.f;
    hpIcon->anchor = TOP_LEFT;
    hpIcon->verticalTranslate = -10.f / 28.f;
    hpIcon->horizontalTranslate = 33.f / 113.f;
    hpIcon->textureId = canvas->LoadUITexture("HP.png");

    cUIText* name = new cUIText();
    name->fontName = "Truth And Ideals - Fighting Ideals-Normal.ttf";
    name->text = Player::party[0].name;
    name->heightPercent = 9.f / 28.f;
    name->anchor = TOP_LEFT;
    name->horizontalTranslate = 10.f / 113.f;
    AddChild(name);
    Manager::ui.CreateTextDataBuffer(name);

    cUIImage* expIcon = new cUIImage();
    expIcon->aspectRatio = 7.f / 24.f;
    expIcon->heightPercent = 7.f / 28.f;
    expIcon->anchor = BOTTOM_LEFT;
    expIcon->horizontalTranslate = 11.f / 113.f;
    expIcon->verticalTranslate = 3.f / 28.f;
    expIcon->textureId = canvas->LoadUITexture("EXP.png");

    if (Player::party[0].gender != Pokemon::NO_GENDER)
    {
        cUIImage* genderIcon = new cUIImage();
        genderIcon->aspectRatio = 10.f / 7.f;
        genderIcon->heightPercent = 10.f / 28.f;
        genderIcon->anchor = TOP_RIGHT;
        genderIcon->horizontalTranslate = -40.f / 113.f;

        if (Player::party[0].gender == Pokemon::FEMALE) genderIcon->textureId = canvas->LoadUITexture("FemaleIcon.png");
        else genderIcon->textureId = canvas->LoadUITexture("MaleIcon.png");
        AddChild(genderIcon);
    }

    AddChild(expIcon);
    AddChild(hpIcon);
    AddChild(playeHb);
    AddChild(bg);
}

void cPlayerBattleInfo::UpdatePlayerInfo()
{
    Player::party[0].currHealth /= 2.f;
    float healthPercent = (float)Player::party[0].currHealth / (float)Player::party[0].maxHealth;
    playeHb->UpdateHealthBar(healthPercent);
}

const float MENU_BUTTON_RATIO = 46.f / 126.f;

cBattleCanvas::cBattleCanvas()
{
    cUIWidget* menuBtnContainer = new cUIWidget();
    menuBtnContainer->anchor = BOTTOM_RIGHT;
    menuBtnContainer->aspectRatio = MENU_BUTTON_RATIO;
    menuBtnContainer->heightPercent = 1.f / 4.f; // change this for size

    fightButton = new cUIImage();
    fightButton->anchor = TOP_LEFT;
    fightButton->heightPercent = 0.5f;
    fightButton->aspectRatio = MENU_BUTTON_RATIO;
    fightButton->textureId = LoadUITexture("button.png");
    fightButton->hoveredTextureId = LoadUITexture("button2.png");
    menuBtnContainer->AddChild(fightButton);

    cUIText* fightText = new cUIText();
    fightText->fontName = "Truth And Ideals-Normal.ttf";
    fightText->text = "Fight";
    fightText->heightPercent = 0.3f;
    fightButton->AddChild(fightText);
    Manager::ui.CreateTextDataBuffer(fightText);

    pokemonButton = new cUIImage();
    pokemonButton->anchor = TOP_RIGHT;
    pokemonButton->heightPercent = 0.5f;
    pokemonButton->aspectRatio = MENU_BUTTON_RATIO;
    pokemonButton->textureId = LoadUITexture("button.png");
    pokemonButton->hoveredTextureId = LoadUITexture("button2.png");
    menuBtnContainer->AddChild(pokemonButton);

    cUIText* pkmText = new cUIText();
    pkmText->fontName = "Truth And Ideals-Normal.ttf";
    pkmText->text = "Pokemon";
    pkmText->heightPercent = 0.3f;
    pokemonButton->AddChild(pkmText);
    Manager::ui.CreateTextDataBuffer(pkmText);

    bagButton = new cUIImage();
    bagButton->anchor = BOTTOM_LEFT;
    bagButton->heightPercent = 0.5f;
    bagButton->aspectRatio = MENU_BUTTON_RATIO;
    bagButton->textureId = LoadUITexture("button.png");
    bagButton->hoveredTextureId = LoadUITexture("button2.png");
    menuBtnContainer->AddChild(bagButton);

    cUIText* bagText = new cUIText();
    bagText->fontName = "Truth And Ideals-Normal.ttf";
    bagText->text = "Bag";
    bagText->heightPercent = 0.3f;
    bagButton->AddChild(bagText);
    Manager::ui.CreateTextDataBuffer(bagText);

    runButton = new cUIImage();
    runButton->anchor = BOTTOM_RIGHT;
    runButton->heightPercent = 0.5f;
    runButton->aspectRatio = MENU_BUTTON_RATIO;
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

    //cUIWidget* battleHudContainer = new cUIWidget();
    //battleHudContainer->aspectRatio = (float)Manager::camera.SCR_HEIGHT / (float)Manager::camera.SCR_WIDTH;
    //battleHudContainer->heightPercent = 0.8f;

    pbi = new cPlayerBattleInfo(this);
    pbi->heightPercent = 0.2f;
    pbi->anchor = BOTTOM_LEFT;
    //battleHudContainer->AddChild(pbi);

    anchoredWidgets.push_back(pbi);
    //anchoredWidgets.push_back(battleHudContainer);
}

void cBattleCanvas::ConfirmAction()
{
    if (currFocus == fightButton)
    {
        pbi->UpdatePlayerInfo();
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

cPartyMemberButton::cPartyMemberButton(cUICanvas* canvas, int memberNum)
{
    Pokemon::sIndividualData& member = Player::party[memberNum];

    textureId = canvas->LoadUITexture("button.png");
    hoveredTextureId = canvas->LoadUITexture("button2.png");
    aspectRatio = MENU_BUTTON_RATIO;
    heightPercent = 1.f / 3.f;

    cUIText* memberText = new cUIText();
    memberText->fontName = "Truth And Ideals-Normal.ttf";
    memberText->text = member.name;
    memberText->heightPercent = 0.3f;
    AddChild(memberText);
    Manager::ui.CreateTextDataBuffer(memberText);

    cUIImage* icon = new cUIImage();
    icon->anchor = MIDDLE_LEFT;
    icon->aspectRatio = 3.f / 4.f;
    icon->heightPercent = 3.f / 4.f;
    icon->textureId = canvas->LoadUITexture(member.MakeIconTextureName(), "assets/pokemon/" + Pokemon::MakeDexNumberFolderName(member.nationalDexNumber) + "/");
    AddChild(icon);
}

cPartyBackground::cPartyBackground(cUICanvas* canvas)
{
    scroll = std::make_shared<cVec2Animation>(translate);
    scroll->AddKeyFrame(sKeyFrameVec2(5.f, glm::vec2(5.f)));
    scroll->isRepeat = true;
    Manager::animation.AddAnimation(scroll);
}

cPartyBackground::~cPartyBackground()
{
    Manager::animation.RemoveAnimation(scroll);
}

cPartyCanvas::cPartyCanvas()
{
    cPartyBackground* background = new cPartyBackground(this);
    background->textureId = LoadUITexture("background.png");
    background->aspectRatio = (float)Manager::camera.SCR_HEIGHT / (float)Manager::camera.SCR_WIDTH;
    background->useScreenSpace = true;
    background->screenSpaceRatio = glm::vec2(30.f);

    cUIWidget* membersContainer = new cUIWidget();
    membersContainer->aspectRatio = (46.f * 3.f) / (126.f * 2.f);
    membersContainer->heightPercent = 0.9f;

    member1 = new cPartyMemberButton(this, 0);
    member1->anchor = TOP_LEFT;
    membersContainer->AddChild(member1);

    if (Player::party[1].nationalDexNumber != 0)
    {
        member2 = new cPartyMemberButton(this, 1);
        member2->anchor = TOP_RIGHT;
        member2->SetMoveFocus(member1, LEFT, true);
        membersContainer->AddChild(member2);
    }

    if (Player::party[2].nationalDexNumber != 0)
    {
        member3 = new cPartyMemberButton(this, 2);
        member3->anchor = MIDDLE_LEFT;
        member3->SetMoveFocus(member1, UP, true);
        membersContainer->AddChild(member3);
    }

    if (Player::party[3].nationalDexNumber != 0)
    {
        member4 = new cPartyMemberButton(this, 3);
        member4->anchor = MIDDLE_RIGHT;
        member4->SetMoveFocus(member3, LEFT, true);
        member4->SetMoveFocus(member2, UP, true);
        membersContainer->AddChild(member4);
    }

    if (Player::party[4].nationalDexNumber != 0)
    {
        member5 = new cPartyMemberButton(this, 4);
        member5->anchor = BOTTOM_LEFT;
        member5->SetMoveFocus(member3, UP, true);
        membersContainer->AddChild(member5);
    }

    if (Player::party[5].nationalDexNumber != 0)
    {
        member6 = new cPartyMemberButton(this, 5);
        member6->anchor = BOTTOM_RIGHT;
        member6->SetMoveFocus(member5, LEFT, true);
        member6->SetMoveFocus(member4, UP, true);
        membersContainer->AddChild(member6);
    }

    defaultFocus = member1;

    anchoredWidgets.push_back(membersContainer);
    anchoredWidgets.push_back(background);
}

void cPartyCanvas::ConfirmAction()
{
    if (memberSelected != 0)
    {
        int otherMember = 0;

        if (currFocus == member1) otherMember = 1;
        else if (currFocus == member2) otherMember = 2;
        else if (currFocus == member3) otherMember = 3;
        else if (currFocus == member4) otherMember = 4;
        else if (currFocus == member5) otherMember = 5;
        else if (currFocus == member6) otherMember = 6;

        Player::SwitchPartyMembers(memberSelected, otherMember);
        memberSelected = 0;
    }
    else
    {
        if (currFocus == member1) memberSelected = 1;
        else if (currFocus == member2) memberSelected = 2;
        else if (currFocus == member3) memberSelected = 3;
        else if (currFocus == member4) memberSelected = 4;
        else if (currFocus == member5) memberSelected = 5;
        else if (currFocus == member6) memberSelected = 6;
    }
}

void cPartyCanvas::CancelAction()
{
    Engine::currGameMode = eGameMode::MAP;
    Manager::ui.RemoveCanvas();
}
