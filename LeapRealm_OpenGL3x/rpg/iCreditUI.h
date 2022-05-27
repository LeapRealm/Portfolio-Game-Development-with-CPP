#pragma once

#include "iItemUI.h"
#include "iStd.h"

struct iCreditUI : iItemUI
{
	iImage* imgBtn;
	iImage* imgBg;

	iCreditUI();
	virtual ~iCreditUI() override;

	void setPopup() override;

	void paint(float dt) override;
	void paintAfter(float dt) override;
};

extern iCreditUI* creditUI;

void drawCreditUIBefore(float dt, iPopup* pop);

void loadCreditUI();
void freeCreditUI();
void drawCreditUI(float dt);
bool keyCreditUI(iKeyState state, iPoint p);
void showCreditUI(bool isShow);
