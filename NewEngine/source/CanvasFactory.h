#pragma once

#include "cUIManager.h"

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

class cPartyCanvas : public cUICanvas
{
	cPartyMemberButton* member1;
	cPartyMemberButton* member2;
	cPartyMemberButton* member3;
	cPartyMemberButton* member4;
	cPartyMemberButton* member5;
	cPartyMemberButton* member6;

	int memberSelected = 0;

public:
	cPartyCanvas();
	virtual ~cPartyCanvas();

	virtual void ConfirmAction();
	virtual void CancelAction();
};