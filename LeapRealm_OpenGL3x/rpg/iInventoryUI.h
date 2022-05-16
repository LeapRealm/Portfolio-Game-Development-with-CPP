#pragma once

#include "iItemUI.h"

enum iSortBy
{
	iSortByGrade,
	iSortByPrice,
};

#define InvenItemW 64
#define InvenItemH 64

struct iInventoryUI : iItemUI
{
	iImage** imgBtn;
	iImage* imgBg;

	iImage** imgSlots;
	iStrTex*** strSlots;

	iInventoryUI();
	virtual ~iInventoryUI() override;

	void setPopup() override;
	void setSlotIndexes();

	void sort(iSortBy sortBy, bool isDesc);

	void paint(float dt) override;
	void paintAfter(float dt) override;
};

extern iInventoryUI* inventoryUI;

Texture* methodStrInvenSlot(const char* str);
void drawInventoryUIBefore(float dt, iPopup* pop);

void loadInventoryUI();
void freeInventoryUI();
void drawInventoryUI(float dt);
bool keyInventoryUI(iKeyState state, iPoint p);
void showInventoryUI(bool isShow);
