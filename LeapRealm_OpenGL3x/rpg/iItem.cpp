#include "iItem.h"

iItem** items;

StatData::StatData()
{
	hp = 0.0f;
	damage = 0.0f;
	criticalRate = 0.0f;
	criticalDamage = 0.0f;
	defense = 0.0f;
}

iItem::iItem()
{
	index = 0;
	itemKind = iItemKindEquip;
	 
	image = nullptr;

	name = nullptr;
	desc = nullptr;

	grade = iItemGradeCommon;
	price = 0;
	stackCntMax = 0;
}

iItem::~iItem()
{
	delete image;
	delete name;
	delete desc;
}

iEquipItem::iEquipItem()
{
	equipKind = iEquipKindSword;
}

iConsumeItem::iConsumeItem()
{
	duration = 0.0f;
}

void loadItem()
{
	items = new iItem * [ItemCnt];

	// sword
	for (int i = 0; i < 6; i++)
	{
		int index = i;
		items[index] = new iEquipItem();

		items[index]->index = index;
		items[index]->itemKind = iItemKindEquip;
		items[index]->name = new iString("sword_%d", i);
		items[index]->desc = new iString("sword_%d_desc", i);
		items[index]->grade = (iItemGrade)(i / 2);
		items[index]->price = (i + 1) * 100;
		items[index]->stackCntMax = 1;
		((iEquipItem*)items[index])->equipKind = iEquipKindSword;
		items[index]->image = iGraphics::createIgImage("assets/ui/item/sword_icon_%d.png", i);
	}

	// shield
	for (int i = 0; i < 6; i++)
	{
		int index = i + 6;
		items[index] = new iEquipItem();

		items[index]->index = index;
		items[index]->itemKind = iItemKindEquip;
		items[index]->name = new iString("shield_%d", i);
		items[index]->desc = new iString("shield_%d_desc", i);
		items[index]->grade = (iItemGrade)(i / 2);
		items[index]->price = (i + 1) * 100;
		items[index]->stackCntMax = 1;
		((iEquipItem*)items[index])->equipKind = iEquipKindShield;
		items[index]->image = iGraphics::createIgImage("assets/ui/item/shield_icon_%d.png", i);
	}

	// helmet
	for (int i = 0; i < 6; i++)
	{
		int index = i + 12;
		items[index] = new iEquipItem();

		items[index]->index = index;
		items[index]->itemKind = iItemKindEquip;
		items[index]->name = new iString("helmet_%d", i);
		items[index]->desc = new iString("helmet_%d_desc", i);
		items[index]->grade = (iItemGrade)(i / 2);
		items[index]->price = (i + 1) * 100;
		items[index]->stackCntMax = 1;
		((iEquipItem*)items[index])->equipKind = iEquipKindHelmet;
		items[index]->image = iGraphics::createIgImage("assets/ui/item/helmet_icon_%d.png", i);
	}

	// chest
	for (int i = 0; i < 6; i++)
	{
		int index = i + 18;
		items[index] = new iEquipItem();

		items[index]->index = index;
		items[index]->itemKind = iItemKindEquip;
		items[index]->name = new iString("chest_%d", i);
		items[index]->desc = new iString("chest_%d_desc", i);
		items[index]->grade = (iItemGrade)(i / 2);
		items[index]->price = (i + 1) * 100;
		items[index]->stackCntMax = 1;
		((iEquipItem*)items[index])->equipKind = iEquipKindChest;
		items[index]->image = iGraphics::createIgImage("assets/ui/item/chest_icon_%d.png", i);
	}

	// potion
	for (int i = 0; i < 4; i++)
	{
		int index = i + 24;
		items[index] = new iConsumeItem();

		items[index]->index = index;
		items[index]->itemKind = iItemKindConsume;
		items[index]->name = new iString("potion_%d", i);
		items[index]->desc = new iString("potion_%d_desc", i);
		items[index]->grade = (iItemGrade)(i % 2);
		items[index]->price = (i + 1) * 100;
		items[index]->stackCntMax = 3 + i;
		((iConsumeItem*)items[index])->duration = (i % 2 + 1) * 3.0f;
		items[index]->image = iGraphics::createIgImage("assets/ui/item/potion_icon_%d.png", i);
	}
}

void freeItem()
{
	for (int i = 0; i < ItemCnt; i++)
		delete items[i];

	delete items;
}
