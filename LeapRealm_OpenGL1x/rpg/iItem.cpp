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

	name = new iString("");
	desc = new iString("");

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
		items[i] = new iEquipItem();

		items[i]->index = i;
		items[i]->itemKind = iItemKindEquip;
		items[i]->name = new iString("sword_%d", i);
		items[i]->desc = new iString("sword_%d_desc", i);
		items[i]->grade = (iItemGrade)(i / 2);
		items[i]->price = (i + 1) * 100;
		items[i]->stackCntMax = 1;
		((iEquipItem*)items[i])->equipKind = iEquipKindSword;

		items[i]->image = iGraphics::createIgImage("assets/ui/item/sword_icon_%d.png", i);
	}
	

	// shield
	for (int i = 0; i < 6; i++)
	{
		items[i + 6] = new iEquipItem();

		items[i + 6]->index = i + 6;
		items[i + 6]->itemKind = iItemKindEquip;
		items[i + 6]->name = new iString("shield_%d", i);
		items[i + 6]->desc = new iString("shield_%d_desc", i);
		items[i + 6]->grade = (iItemGrade)(i / 2);
		items[i + 6]->price = (i + 1) * 100;
		items[i + 6]->stackCntMax = 1;
		((iEquipItem*)items[i + 6])->equipKind = iEquipKindShield;

		items[i + 6]->image = iGraphics::createIgImage("assets/ui/item/shield_icon_%d.png", i);
	}

	// helmet
	for (int i = 0; i < 6; i++)
	{
		items[i + 12] = new iEquipItem();

		items[i + 12]->index = i + 12;
		items[i + 12]->itemKind = iItemKindEquip;
		items[i + 12]->name = new iString("helmet_%d", i);
		items[i + 12]->desc = new iString("helmet_%d_desc", i);
		items[i + 12]->grade = (iItemGrade)(i / 2);
		items[i + 12]->price = (i + 1) * 100;
		items[i + 12]->stackCntMax = 1;
		((iEquipItem*)items[i + 12])->equipKind = iEquipKindHelmet;

		items[i + 12]->image = iGraphics::createIgImage("assets/ui/item/helmet_icon_%d.png", i);
	}

	// chest
	for (int i = 0; i < 6; i++)
	{
		items[i + 18] = new iEquipItem();

		items[i + 18]->index = i + 18;
		items[i + 18]->itemKind = iItemKindEquip;
		items[i + 18]->name = new iString("chest_%d", i);
		items[i + 18]->desc = new iString("chest_%d_desc", i);
		items[i + 18]->grade = (iItemGrade)(i / 2);
		items[i + 18]->price = (i + 1) * 100;
		items[i + 18]->stackCntMax = 1;
		((iEquipItem*)items[i + 18])->equipKind = iEquipKindChest;

		items[i + 18]->image = iGraphics::createIgImage("assets/ui/item/chest_icon_%d.png", i);
	}

	// potion
	for (int i = 0; i < 4; i++)
	{
		items[i + 24] = new iConsumeItem();

		items[i + 24]->index = i + 24;
		items[i + 24]->itemKind = iItemKindConsume;
		items[i + 24]->name = new iString("potion_%d", i);
		items[i + 24]->desc = new iString("potion_%d_desc", i);
		items[i + 24]->grade = (iItemGrade)(i % 2);
		items[i + 24]->price = (i + 1) * 100;
		items[i + 24]->stackCntMax = 5;
		((iConsumeItem*)items[i + 24])->duration = (i % 2 + 1) * 3.0f;

		items[i + 24]->image = iGraphics::createIgImage("assets/ui/item/potion_icon_%d.png", i);
	}
}

void freeItem()
{
	for (int i = 0; i < ItemCnt; i++)
		delete items[i];

	delete items;
}
