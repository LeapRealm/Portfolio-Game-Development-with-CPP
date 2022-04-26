#pragma once

#include "iItemUI.h"
#include "iStd.h"

#define EquipSlotMaxCnt 4

#define EquipItemX 2
#define EquipItemY 2

#define EquipItemW 128
#define EquipItemH 128

struct iEquipUI : iItemUI
{
	iImage* imgBtn;
	iImage* imgBg;

	iImage** imgSlots;
	iStrTex*** strSlots;

	iEquipUI();
	virtual ~iEquipUI() override;

	void setPopup() override;

	void equip();
	void unequip();

	void paint(float dt) override;
	void paintAfter(float dt) override;
};

extern iEquipUI* equipUI;

Texture* methodStrEquipSlot(const char* str);
void drawEquipUIBefore(float dt, iPopup* pop);

void loadEquipUI();
void freeEquipUI();
void drawEquipUI(float dt);
bool keyEquipUI(iKeyState state, iPoint p);
void showEquipUI(bool isShow);
