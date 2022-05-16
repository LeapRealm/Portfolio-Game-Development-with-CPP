#include "iItemUI.h"

#include "iEquipUI.h"
#include "iInventoryUI.h"

iPoint selectedPos, mousePosition;

iItemUI::iItemUI(int slotMaxCnt)
{
	popup = new iPopup();

	this->slotMaxCnt = slotMaxCnt;
	slotIndexes = new int[this->slotMaxCnt];

	isClicked = false;
	selectedSlot = selectingSlot = -1;
}

iItemUI::~iItemUI()
{
	delete slotIndexes;

	delete popup;
}

void iItemUI::swap(int* srcSlot, int* dstSlot)
{
	int t = *srcSlot;
	*srcSlot = *dstSlot;
	*dstSlot = t;
}

void resetFlags()
{
	inventoryUI->selectedSlot = -1;
	inventoryUI->selectingSlot = -1;
	equipUI->selectedSlot = -1;
	equipUI->selectingSlot = -1;
}
