#pragma once

#include "iItemUI.h"
#include "iStd.h"

struct iQuickUI : iItemUI
{
	iImage* imgBtn;
	iImage* imgBg;

	iQuickUI();
	virtual ~iQuickUI() override;

	void setPopup() override;

	void paint(float dt) override;
	void paintAfter(float dt) override;
};

extern iQuickUI* quickUI;

void drawQuickUIBefore(float dt, iPopup* pop);

void loadQuickUI();
void freeQuickUI();
void drawQuickUI(float dt);
bool keyQuickUI(iKeyState state, iPoint p);
void showQuickUI(bool isShow);
