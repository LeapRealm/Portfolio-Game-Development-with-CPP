#include "iInventory.h"

#include "iInventoryUI.h"

iSlot::iSlot()
{
	itemIndex = 0;
	itemCount = 0;
	isEquipped = false;
}

iInventory::iInventory()
{
	slots = new iSlot[InvenSlotMaxCnt];
}

iInventory::~iInventory()
{
	delete slots;
}

void iInventory::_add(int itemIndex, int& itemCount, bool isNewInsert)
{
	bool condition;
		
	for (int i = 0; i < InvenSlotMaxCnt; i++)
	{
		iSlot* slot = &slots[i];

		if (isNewInsert == false)
			condition = (slot->itemCount != 0 && slot->itemIndex == itemIndex);
		else
			condition = (slot->itemCount == 0);

		if (condition)
		{
			int prevItemCount = slot->itemCount;
			slot->itemCount += itemCount;
			slot->itemIndex = itemIndex;

			if (slot->itemCount > items[slot->itemIndex]->stackCntMax)
			{
				slot->itemCount = items[slot->itemIndex]->stackCntMax;
				itemCount -= (slot->itemCount - prevItemCount);
			}
			else
			{
				itemCount = 0;
				return;
			}
		}
	}
}

bool iInventory::add(int itemIndex, int itemCount)
{
	iSlot backupSlot[InvenSlotMaxCnt];
	memcpy(backupSlot, slots, sizeof(iSlot) * InvenSlotMaxCnt);

	_add(itemIndex, itemCount, false);

	if (itemCount)
		_add(itemIndex, itemCount, true);

	if (itemCount)
	{
		memcpy(slots, backupSlot, sizeof(iSlot) * InvenSlotMaxCnt);
		return false;
	}

	return true;
}

void iInventory::move(int selectedSlot, int selectingSlot)
{
	if (selectedSlot == selectingSlot)
		return;

	int srcSlotIndex = inventoryUI->slotIndexes[selectedSlot];
	int dstSlotIndex = inventoryUI->slotIndexes[selectingSlot];

	iSlot* srcSlot = &slots[srcSlotIndex];
	iSlot* dstSlot = &slots[dstSlotIndex];

	if (dstSlot->itemCount != 0 && 
		srcSlot->itemIndex == dstSlot->itemIndex && 
		dstSlot->itemCount < items[dstSlot->itemIndex]->stackCntMax)
	{
		int itemCount = srcSlot->itemCount;
		int prevItemCount = dstSlot->itemCount;
		dstSlot->itemCount += itemCount;

		if (dstSlot->itemCount > items[dstSlot->itemIndex]->stackCntMax)
		{
			dstSlot->itemCount = items[dstSlot->itemIndex]->stackCntMax;
			itemCount -= (dstSlot->itemCount - prevItemCount);
		}
		else
		{
			itemCount = 0;
		}

		srcSlot->itemCount = itemCount;
	}
	else
	{
		inventoryUI->swap(&inventoryUI->slotIndexes[selectedSlot], 
						  &inventoryUI->slotIndexes[selectingSlot]);
	}
}

bool iInventory::remove(int itemIndex, int itemCount)
{
	iSlot backupSlot[InvenSlotMaxCnt];
	memcpy(backupSlot, slots, sizeof(iSlot) * InvenSlotMaxCnt);

	for (int i = 0; i < InvenSlotMaxCnt; i++)
	{
		iSlot* slot = &slots[i];

		if (slot->itemIndex == itemIndex && slot->itemCount > 0)
		{
			if (slot->itemCount >= itemCount)
			{
				slot->itemCount -= itemCount;
				itemCount = 0;
				break;
			}

			itemCount -= slot->itemCount;
			slot->itemCount = 0;
		}
	}

	if (itemCount)
	{
		memcpy(slots, backupSlot, sizeof(iSlot) * InvenSlotMaxCnt);
		return false;
	}
	return true;
}

iInventory* inventory;

void loadInventory()
{
	inventory = new iInventory();

	inventory->add(0, 1);
	inventory->add(2, 1);
	inventory->add(4, 1);
	inventory->add(6, 1);
	inventory->add(8, 1);
	inventory->add(10, 1);
	inventory->add(12, 1);
	inventory->add(14, 1);
	inventory->add(16, 1);
	inventory->add(18, 1);
	inventory->add(20, 1);
	inventory->add(22, 1);

	inventory->slots[20].itemIndex = 25;
	inventory->slots[20].itemCount = 2;

	inventory->slots[21].itemIndex = 25;
	inventory->slots[21].itemCount = 2;

	inventory->slots[22].itemIndex = 25;
	inventory->slots[22].itemCount = 2;

	inventory->slots[24].itemIndex = 26;
	inventory->slots[24].itemCount = 2;

	inventory->slots[25].itemIndex = 26;
	inventory->slots[25].itemCount = 2;

	inventory->slots[26].itemIndex = 26;
	inventory->slots[26].itemCount = 2;
}

void freeInventory()
{
	delete inventory;
}
 