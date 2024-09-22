#pragma once

#include "cUIManager.h"

#include <memory>
#include "cAnimation.h"

class cMenuButtonWidget : public cUIImage
{
public:
	cMenuButtonWidget(cUICanvas* canvas, std::string text, std::string iconFileName);
	virtual ~cMenuButtonWidget() {};
};

class cOverworldCanvas : public cUICanvas
{
public:
	cOverworldCanvas();
	virtual ~cOverworldCanvas() {};

	virtual void ConfirmAction();
	virtual void CancelAction();
};

class cHealthBar : public cUIImage
{
	cUIImage* hbContent;
public:
	cHealthBar(cUICanvas* canvas);
	virtual ~cHealthBar() {};

	void UpdateHealthBar(float healthPercent);
};

class cPlayerBattleInfo : public cUIWidget
{
	cHealthBar* playerHb;
public:
	cPlayerBattleInfo(cUICanvas* canvas);
	virtual ~cPlayerBattleInfo() {};

	void UpdatePlayerInfo();
};

class cEnemyBattleInfo : public cUIWidget
{
	cHealthBar* enemyHb;
public:
	cEnemyBattleInfo(cUICanvas* canvas);
	virtual ~cEnemyBattleInfo() {};

	void UpdateEnemyInfo();
};

class cBattleCanvas : public cUICanvas
{
	cUIImage* fightButton;
	cUIImage* pokemonButton;
	cUIImage* bagButton;
	cUIImage* runButton;

	cPlayerBattleInfo* pbi;
	cEnemyBattleInfo* ebi;

public:
	cBattleCanvas();
	virtual ~cBattleCanvas() {};

	virtual void ConfirmAction();
	virtual void CancelAction();
};

class cPartyMemberButton : public cUIImage
{
public:
	cPartyMemberButton(cUICanvas* canvas, int memberNum);
	virtual ~cPartyMemberButton() {};
};

class cPartyBackground : public cUIImage
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
	virtual ~cPartyCanvas() {};

	virtual void ConfirmAction();
	virtual void CancelAction();
};