#pragma once

#include "iStd.h"

#define StrPadding 50

struct iItemUI
{
	iItemUI(int slotMaxCnt);
	virtual ~iItemUI();

	iPopup* popup;

	int slotMaxCnt;
	int* slotIndexes;

	bool isClicked;
	int selectedSlot, selectingSlot;

	virtual void setPopup() = 0;

	void swap(int* srcSlot, int* dstSlot);

	virtual void paint(float dt) = 0;
	virtual void paintAfter(float dt) = 0;
};

extern iPoint selectedPos, mousePosition;

void resetFlags();
