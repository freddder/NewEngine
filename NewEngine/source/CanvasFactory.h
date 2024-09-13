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

	virtual void CancelAction();
};

class cBattleCanvas : public cUICanvas
{
	cUIStaticImage* fightButton;
	cUIStaticImage* runButton;

public:
	cBattleCanvas();
	virtual ~cBattleCanvas();

	virtual void ConfirmAction();
	virtual void CancelAction();
};
