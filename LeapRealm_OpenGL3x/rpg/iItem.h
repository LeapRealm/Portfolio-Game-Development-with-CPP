#pragma once

#include "iStd.h"

#define ItemCnt 28

enum iItemKind
{
	iItemKindEquip,
	iItemKindConsume,
};

enum iEquipKind
{
	iEquipKindSword,
	iEquipKindShield,
	iEquipKindHelmet,
	iEquipKindChest,
};
 
enum iItemGrade
{
	iItemGradeCommon,	 // Green
	iItemGradeRare,		 // Purple
	iItemGradeLegendary, // Orange
};

struct StatData
{
	StatData();

	float hp;
	float damage;
	float criticalRate;
	float criticalDamage;
	float defense;
};

struct iItem
{
	iItem();
	virtual ~iItem();

	int index;
	iItemKind itemKind;

	igImage* image;

	iString* name;
	iString* desc;

	iItemGrade grade;
	int price;
	int stackCntMax;
};

struct iEquipItem : iItem
{
	iEquipItem();

	iEquipKind equipKind;
	StatData statData;
};

struct iConsumeItem : iItem
{
	iConsumeItem();

	float duration;
	StatData statData;
};

extern iItem** items;

void loadItem();
void freeItem();
