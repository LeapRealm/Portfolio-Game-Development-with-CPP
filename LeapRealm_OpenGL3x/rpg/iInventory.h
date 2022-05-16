#pragma once

#include "iItem.h"

#define InvenItemX 5
#define InvenItemY 6
#define InvenSlotMaxCnt (InvenItemX * InvenItemY)

struct iSlot
{
	iSlot();

	int itemIndex;
	int itemCount;
	int isEquipped;
};

struct iInventory
{
	iInventory();
	virtual ~iInventory();

	iSlot* slots;

	void _add(int itemIndex, int& itemCount, bool isNewInsert);
	bool add(int itemIndex, int itemCount);

	bool remove(int itemIndex, int itemCount);

	void move(int selectedSlot, int selectingSlot);
};

extern iInventory* inventory;

void loadInventory();
void freeInventory();
