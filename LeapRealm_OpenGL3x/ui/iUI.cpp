#include "iUI.h"

#include "iDimmed.h"
#include "iEquipUI.h"
#include "iInventory.h"
#include "iInventoryUI.h"
#include "iItem.h"
#include "iPause.h"
#include "iQuickUI.h"
#include "iSetting.h"
#include "iSound.h"
#include "iTopMenu.h"

iLinkedList* listLayer;
Layer* inventoryLayer;
Layer* equipLayer;
Layer* quickLayer;

void loadUI()
{
	loadTopMenu();

	loadItem();
	loadInventory();
	loadInventoryUI();

	loadEquipUI();

	loadQuickUI();

	createDimmed();

	loadPause();
	loadSetting();

	showTopMenu(true);

	listLayer = new iLinkedList();
	inventoryLayer = new Layer();
	equipLayer = new Layer();
	quickLayer = new Layer();

	inventoryLayer->draw = drawInventoryUI;
	inventoryLayer->key = keyInventoryUI;
	equipLayer->draw = drawEquipUI;
	equipLayer->key = keyEquipUI;
	quickLayer->draw = drawQuickUI;
	quickLayer->key = keyQuickUI;

	listLayer->addObject(inventoryLayer);
	listLayer->addObject(equipLayer);
	listLayer->addObject(quickLayer);
}

void freeUI()
{
	delete quickLayer;
	delete equipLayer;
	delete inventoryLayer;
	delete listLayer;

	freeSetting();
	freePause();

	freeDimmed();

	freeQuickUI();

	freeEquipUI();

	freeInventoryUI();
	freeInventory();
	freeItem();

	freeTopMenu();
}

void drawUI(float dt)
{
	drawTopMenu(dt);

	for (int i = 0; i < listLayer->count; i++)
		((Layer*)listLayer->getObjectByIndex(i))->draw(dt);

	inventoryUI->paintAfter(dt);
	equipUI->paintAfter(dt);

	drawDimmed(dt);

	drawPause(dt);
	drawSetting(dt);
}

bool keyUI(iKeyState state, iPoint p)
{
	if (keyPause(state, p) || 
		keySetting(state, p))
		return true;

	for (int i = listLayer->count - 1; i > -1; i--)
	{
		if (((Layer*)listLayer->getObjectByIndex(i))->key(state, p))
			return true;
	}

	if (keyTopMenu(state, p))
		return true;

	return false;
}
