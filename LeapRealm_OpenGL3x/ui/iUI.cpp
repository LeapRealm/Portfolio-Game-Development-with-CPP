#include "iUI.h"

#include "iCaptionUI.h"
#include "iDimmedUI.h"
#include "iEquipUI.h"
#include "iInventory.h"
#include "iInventoryUI.h"
#include "iItem.h"
#include "iLogUI.h"
#include "iPauseUI.h"
#include "iCreditUI.h"
#include "iSettingUI.h"
#include "iSound.h"
#include "iTopMenuUI.h"

iLinkedList* listLayer;
Layer* inventoryLayer;
Layer* equipLayer;
Layer* testLayer;

void loadUI()
{
	loadLogUI();

	loadTopMenuUI();

	loadItem();
	loadInventory();
	loadInventoryUI();

	loadEquipUI();

	loadCreditUI();

	createDimmedUI();

	loadPauseUI();
	loadSettingUI();

	loadCaptionUI();

	showTopMenuUI(true);
	showLogUI(true);

	listLayer = new iLinkedList();
	inventoryLayer = new Layer();
	equipLayer = new Layer();
	testLayer = new Layer();

	inventoryLayer->draw = drawInventoryUI;
	inventoryLayer->key = keyInventoryUI;
	equipLayer->draw = drawEquipUI;
	equipLayer->key = keyEquipUI;
	testLayer->draw = drawCreditUI;
	testLayer->key = keyCreditUI;

	listLayer->addObject(inventoryLayer);
	listLayer->addObject(equipLayer);
	listLayer->addObject(testLayer);
}

void freeUI()
{
	delete testLayer;
	delete equipLayer;
	delete inventoryLayer;
	delete listLayer;

	freeCaptionUI();

	freeSettingUI();
	freePauseUI();

	freeDimmedUI();

	freeCreditUI();

	freeEquipUI();

	freeInventoryUI();
	freeInventory();
	freeItem();

	freeTopMenuUI();

	freeLogUI();
}

void drawUI(float dt)
{
	drawLogUI(dt);

	drawTopMenuUI(dt);

	for (int i = 0; i < listLayer->count; i++)
		((Layer*)listLayer->getObjectByIndex(i))->draw(dt);

	inventoryUI->paintAfter(dt);
	equipUI->paintAfter(dt);

	drawDimmedUI(dt);

	drawPauseUI(dt);
	drawSettingUI(dt);

	drawCaptionUI(dt);
}

bool keyUI(iKeyState state, iPoint p)
{
	if (keyPauseUI(state, p) || 
		keySettingUI(state, p))
		return true;

	for (int i = listLayer->count - 1; i > -1; i--)
	{
		if (((Layer*)listLayer->getObjectByIndex(i))->key(state, p))
			return true;
	}

	if (keyTopMenuUI(state, p))
		return true;

	if (keyLogUI(state, p))
		return true;

	return false;
}
