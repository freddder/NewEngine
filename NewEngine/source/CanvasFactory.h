#pragma once

#include "cUIManager.h"

#include <memory>
#include "cAnimation.h"

class cMenuButtonWidget : public cUIStaticImage
{
public:
	cMenuButtonWidget(cUICanvas* canvas, std::string text, std::string iconFileName);
	virtual ~cMenuButtonWidget();
};

class cOverworldCanvas : public cUICanvas
{
public:
	cOverworldCanvas();
	virtual ~cOverworldCanvas();

	virtual void ConfirmAction();
	virtual void CancelAction();
};

class cBattleCanvas : public cUICanvas
{
	cUIStaticImage* fightButton;
	cUIStaticImage* pokemonButton;
	cUIStaticImage* bagButton;
	cUIStaticImage* runButton;

public:
	cBattleCanvas();
	virtual ~cBattleCanvas();

	virtual void ConfirmAction();
	virtual void CancelAction();
};

class cPartyMemberButton : public cUIStaticImage
{
public:
	cPartyMemberButton(cUICanvas* canvas, int memberNum);
	virtual ~cPartyMemberButton();
};

class cPartyBackground : public cUIStaticImage
{
	std::shared_ptr<cVec2Animation> scroll;
public:
	cPartyBackground(cUICanvas* canvas);
	virtual ~cPartyBackground();
};

class cPartyCanvas : public cUICanvas
{
	cPartyMemberButton* member1 = nullptr;
	cPartyMemberButton* member2 = nullptr;
	cPartyMemberButton* member3 = nullptr;
	cPartyMemberButton* member4 = nullptr;
	cPartyMemberButton* member5 = nullptr;
	cPartyMemberButton* member6 = nullptr;

	int memberSelected = 0;

public:
	cPartyCanvas();
	virtual ~cPartyCanvas();

	virtual void ConfirmAction();
	virtual void CancelAction();
};