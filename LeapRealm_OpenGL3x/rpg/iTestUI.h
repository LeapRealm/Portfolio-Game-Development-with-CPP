#pragma once

#include "iItemUI.h"
#include "iStd.h"

struct iTestUI : iItemUI
{
	iImage* imgBtn;
	iImage* imgBg;

	iTestUI();
	virtual ~iTestUI() override;

	void setPopup() override;

	void paint(float dt) override;
	void paintAfter(float dt) override;
};

extern iTestUI* testUI;

void drawTestUIBefore(float dt, iPopup* pop);

void loadTestUI();
void freeTestUI();
void drawTestUI(float dt);
bool keyTestUI(iKeyState state, iPoint p);
void showTestUI(bool isShow);
