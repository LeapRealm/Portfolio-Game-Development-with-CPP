#include "iInventoryUI.h"

#include "iEquipUI.h"
#include "iInventory.h"
#include "iTopMenu.h"
#include "iUI.h"

iInventoryUI::iInventoryUI(): iItemUI(InvenSlotMaxCnt)
{
	setSlotIndexes();

	imgBtn = new iImage * [3];
	imgSlots = new iImage * [slotMaxCnt];

	strSlots = new iStrTex * * [slotMaxCnt];
	for (int i = 0; i < slotMaxCnt; i++)
		strSlots[i] = new iStrTex * [2];
}

iInventoryUI::~iInventoryUI()
{
	delete imgBtn;
	delete imgSlots;

	for (int i = 0; i < slotMaxCnt; i++)
	{
		for (int j = 0; j < 2; j++)
			delete strSlots[i][j];
		delete strSlots[i];
	}
	delete strSlots;
}

void iInventoryUI::setPopup()
{
	iGraphics* g = new iGraphics();

	igImage* bg = iGraphics::createIgImage("assets/ui/inventory_bg.png");
	igImage* btn = iGraphics::createIgImage("assets/ui/inventory_btn.png");
	igImage* close = iGraphics::createIgImage("assets/ui/close_icon.png");

	iSize size = iSizeMake(380, 530);
	g->init(size);

	setRGBA(1, 1, 1, 1);
	g->drawIgImage(bg, 0, 0, size.width / 334, size.height / 424, TOP | LEFT);

	setStringName("assets/fonts/NanumSquareEB.ttf");
	setStringRGBA(0.96f, 0.83f, 0.53f, 1);
	setStringBorder(0);

	float stringSize = 25;
	setStringSize(stringSize);
	iRect rt = rectOfString("인벤토리");
	if (rt.size.width > (size.width - 50))
		setStringSize(stringSize * (size.width - 50) / rt.size.width);

	g->drawString(size.width / 2, 47, VCENTER | HCENTER, "인벤토리");

	// Inven Slot Layout Lines
	setLineWidth(1);
	setRGBA(0.47f, 0.42f, 0.46f, 1);
	int offX = (size.width - InvenItemW * InvenItemX) / 2;
	int offY = 90;
	for (int i = 0; i < InvenItemX + 1; i++)
		g->drawLine(offX + InvenItemW * i, offY, offX + InvenItemW * i, offY + InvenItemH * InvenItemY);
	for (int i = 0; i < InvenItemY + 1; i++)
		g->drawLine(offX, offY + InvenItemH * i, offX + InvenItemW * InvenItemX, offY + InvenItemH * i);

	Texture* tex = g->getTexture();
	iImage* img = new iImage();
	img->addTexture(tex);
	freeTexture(tex);

	popup->addImage(img);
	imgBg = img;

	// btn [0]/[1]
	const char* strBtn[2] = { "오름차순", "내림차순" };
	setStringName("assets/fonts/NanumSquareB.ttf");
	setStringBorder(0);
	setStringRGBA(0.09f, 0.09f, 0.09f, 1);
	size = iSizeMake(158 * 0.8f, 56 * 0.5f);

	stringSize = 15;
	setStringSize(stringSize);

	for (int i = 0; i < 2; i++)
	{
		img = new iImage();
		for (int j = 0; j < 2; j++)
		{
			g->init(size);

			if (j == 0)
				setRGBA(1, 1, 1, 1);
			else
				setRGBA(0.78f, 0.78f, 0.78f, 1);

			g->drawIgImage(btn, 0, 0, 0.79f, 0.49f, TOP | LEFT);

			setStringSize(stringSize);
			rt = rectOfString(strBtn[i]);
			if (rt.size.width > (size.width - 25))
				setStringSize(stringSize * (size.width - 25) / rt.size.width);

			g->drawString(size.width / 2, size.height / 2 - 1, VCENTER | HCENTER, strBtn[i]);

			tex = g->getTexture();
			img->addTexture(tex);
			freeTexture(tex);
		}

		img->position = iPointMake(53 + 150 * i, 484);
		popup->addImage(img);
		imgBtn[i] = img;
	}

	// btn [2]
	img = new iImage();
	size = iSizeMake(40 * 0.6f, 40 * 0.6f);
	setRGBA(1, 1, 1, 1);
	for (int i = 0; i < 2; i++)
	{
		g->init(size);

		if (i == 0)
			setRGBA(1, 1, 1, 1);
		else
			setRGBA(0.78f, 0.7f, 0.7f, 1);

		g->drawIgImage(close, 0, 0, 0.6f, 0.6f, TOP | LEFT);

		tex = g->getTexture();
		img->addTexture(tex);
		freeTexture(tex);
	}

	img->position = iPointMake(323, 35);
	popup->addImage(img);
	imgBtn[2] = img;

	// Slots
	for (int i = 0; i < slotMaxCnt; i++)
	{
		img = new iImage();
		iSlot* sl = &inventory->slots[slotIndexes[i]];

		for (int j = 0; j < 2; j++)
		{
			iStrTex* st = new iStrTex(methodStrInvenSlot);
			st->setString("%d/%d/%d/%d", sl->itemIndex, sl->isEquipped, sl->itemCount, j);

			img->addTexture(st->tex);
			strSlots[i][j] = st;
		}

		img->position = iPointMake(offX + InvenItemW * (i % InvenItemX),
								   offY + InvenItemH * (i / InvenItemX));
		popup->addImage(img);
		imgSlots[i] = img;
	}

	popup->style = iPopupStyleAlpha;
	popup->openPoint = imgTopMenuBtn[2]->center(topMenuUI->closePoint);
	popup->closePoint = iPointMake((devSize.width - 380) / 2, (devSize.height - 320) / 3);
	popup->methodDrawBefore = drawInventoryUIBefore;

	delete bg;
	delete btn;
	delete close;
	delete g;
}

void iInventoryUI::setSlotIndexes()
{
	int slotIndexCnt = 0;
	for (int i = 0; i < slotMaxCnt; i++)
	{
		if (inventory->slots[i].itemCount)
		{
			slotIndexes[slotIndexCnt] = i;
			slotIndexCnt++;
		}
	}

	for (int i = 0; i < slotMaxCnt; i++)
	{
		if (inventory->slots[i].itemCount == 0)
		{
			slotIndexes[slotIndexCnt] = i;
			slotIndexCnt++;
		}
	}
}

void iInventoryUI::sort(iSortBy sortBy = iSortByGrade, bool isDesc = true)
{
	setSlotIndexes();

	iSort sort;
	sort.init();
	for(int i = 0; i < slotMaxCnt; i++)
	{
		iSlot* slot = &inventory->slots[slotIndexes[i]];

		if (slot->itemCount == 0)
			break;

		iItem* item = items[slot->itemIndex];
		if		(sortBy == iSortByGrade)	sort.add(slotIndexes[i], item->grade);
		else if (sortBy == iSortByPrice)	sort.add(slotIndexes[i], item->price);
	}
	sort.update();

	for(int i = 0; i < sort.sdNum; i++)
		slotIndexes[isDesc ? i : sort.sdNum - 1 - i] = sort.getIndex(i);
}

void iInventoryUI::paint(float dt)
{
	popup->paint(dt);
}

void iInventoryUI::paintAfter(float dt)
{
	if (selectedSlot == -1)
		return;

	Texture* tex = imgSlots[selectedSlot]->tex;
	drawTexture(tex, selectedPos, TOP | LEFT);
}

Texture* methodStrInvenSlot(const char* str)
{
	int lineNum;
	char** line = iString::getStringLine(str, lineNum, '/');
	int itemIndex = atoi(line[0]);
	int isEquipped = atoi(line[1]);
	int itemCount = atoi(line[2]);
	int j = atoi(line[3]);
	iString::freeStringLine(line, lineNum);

	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(InvenItemW, InvenItemH);
	g->init(size);

	if (itemCount)
	{
		iItem* it = items[itemIndex];

		if (j == 0)
		{
			switch (it->grade)
			{
			case iItemGradeCommon:
				setRGBA(0.08f, 0.18f, 0.13f, 1);
				break;

			case iItemGradeRare:
				setRGBA(0.13f, 0.05f, 0.20f, 1);
				break;

			case iItemGradeLegendary:
				setRGBA(0.18f, 0.12f, 0.07f, 1);
				break;
			}
		}
		else
		{
			float m = 0.5f;
			switch (it->grade)
			{
			case iItemGradeCommon:
				setRGBA(0.08f * m, 0.18f * m, 0.13f * m, 0.7f);
				break;

			case iItemGradeRare:
				setRGBA(0.13f * m, 0.05f * m, 0.20f * m, 0.7f);
				break;

			case iItemGradeLegendary:
				setRGBA(0.18f * m, 0.12f * m, 0.07f * m, 0.7f);
				break;
			}
		}
		g->fillRect(0, 0, size.width, size.height);

		setRGBA(0.47f, 0.42f, 0.46f, 1);
		setLineWidth(1);
		g->drawRect(0, 0, size.width, size.height);

		setRGBA(1, 1, 1, 1);
		g->drawIgImage(items[itemIndex]->image, 0, 0, 0.5f, 0.5f, TOP | LEFT);

		setStringSize(InvenItemH * 0.333333f);
		setStringRGBA(1, 1, 1, 1);
		setStringBorder(1);
		setStringBorderRGBA(0, 0, 0, 1);

		if (it->itemKind == iItemKindEquip)
			g->drawString(size.width - 5, size.height - 5, BOTTOM | RIGHT, "%s", (isEquipped ? "E" : " "));
		else if (it->itemKind == iItemKindConsume)
			g->drawString(size.width - 5, size.height - 5, BOTTOM | RIGHT, "%d", itemCount);
	}
	else
	{
		setRGBA(0.5f, 0.5f, 0.5f, 0);
		g->fillRect(0, 0, size.width, size.height);
	}
	Texture* tex = g->getTexture();
	delete g;

	return tex;
}

void drawInventoryUIBefore(float dt, iPopup* pop)
{
	for (int i = 0; i < inventoryUI->slotMaxCnt; i++)
	{
		iSlot* sl = &inventory->slots[inventoryUI->slotIndexes[i]];
		for (int j = 0; j < 2; j++)
			inventoryUI->strSlots[i][j]->setString("%d/%d/%d/%d", sl->itemIndex, sl->isEquipped, sl->itemCount, j);

		inventoryUI->imgSlots[i]->frame = (i == inventoryUI->selectedSlot || i == inventoryUI->selectingSlot);
	}

	for (int i = 0; i < 3; i++)
		inventoryUI->imgBtn[i]->frame = (i == inventoryUI->popup->selected);
}

iInventoryUI* inventoryUI;

void loadInventoryUI()
{
	inventoryUI = new iInventoryUI();
	inventoryUI->setPopup();
}

void freeInventoryUI()
{
	delete inventoryUI;
}

void drawInventoryUI(float dt)
{
	inventoryUI->paint(dt);
}

bool keyInventoryUI(iKeyState state, iPoint p)
{
	if (inventoryUI->popup->isShow == false)
		return false;
	if (inventoryUI->popup->state != iPopupStateProc)
		return true;

	switch (state)
	{
	case iKeyStateBegan:
	{
		if (containPoint(p, inventoryUI->imgBg->rect(inventoryUI->popup->closePoint)))
		{
			listLayer->removeObject(inventoryLayer);
			listLayer->addObject(inventoryLayer);
		}

		if (containPoint(p, iRectMake(inventoryUI->popup->closePoint.x, inventoryUI->popup->closePoint.y, 300, 75)))
		{
			inventoryUI->isClicked = true;
			selectedPos = inventoryUI->popup->closePoint;
			mousePosition = p;
			break;
		}

		for (int i = 0; i < inventoryUI->slotMaxCnt; i++)
		{
			iSlot* is = &inventory->slots[inventoryUI->slotIndexes[i]];
			if (is->itemCount != 0 && containPoint(p, inventoryUI->imgSlots[i]->rect(inventoryUI->popup->closePoint)))
			{
				audioPlay(snd_eff_item_grab);
				inventoryUI->selectedSlot = i;
				selectedPos = inventoryUI->imgSlots[inventoryUI->selectedSlot]->position + inventoryUI->popup->closePoint;
				mousePosition = p;
				inventoryUI->imgSlots[inventoryUI->selectedSlot]->alpha = 0;
				break;
			}
		}

		int s = inventoryUI->popup->selected;
		if (s == -1)
			break;

		if (s == 0)
		{
			audioPlay(snd_eff_mouse_click);
			inventoryUI->sort(iSortByGrade, false);
		}
		else if (s == 1)
		{
			audioPlay(snd_eff_mouse_click);
			inventoryUI->sort(iSortByGrade, true);
		}
		else
		{
			showInventoryUI(false);
		}
		break;
	}

	case iKeyStateMoved:
	{
		if (inventoryUI->isClicked)
		{
			selectedPos += (p - mousePosition);
			inventoryUI->popup->closePoint = selectedPos;
			mousePosition = p;
			break;
		}

		int j;
		if (inventoryUI->selectedSlot == -1)
		{
			j = -1;
			for (int i = 0; i < 3; i++)
			{
				if (containPoint(p, inventoryUI->imgBtn[i]->rect(inventoryUI->popup->closePoint)))
				{
					j = i;
					break;
				}
			}
			inventoryUI->popup->selected = j;
		}
		else
		{
			selectedPos += (p - mousePosition);
			mousePosition = p;

			int j = -1;
			for (int i = 0; i < inventoryUI->slotMaxCnt; i++)
			{
				if (containPoint(p, inventoryUI->imgSlots[i]->rect(inventoryUI->popup->closePoint)))
				{
					j = i;
					break;
				}
			}
			inventoryUI->selectingSlot = j;

			j = -1;
			if (containPoint(p, inventoryUI->imgBg->rect(inventoryUI->popup->closePoint)) == false)
			{
				for (int i = 0; i < equipUI->slotMaxCnt; i++)
				{
					if (containPoint(p, equipUI->imgSlots[i]->rect(equipUI->popup->closePoint)))
					{
						j = i;
						break;
					}
				}
			}
			equipUI->selectingSlot = j;
		}
		break;
	}

	case iKeyStateEnded:
	{
		if (inventoryUI->isClicked)
		{
			inventoryUI->isClicked = false;
			break;
		}

		if (inventoryUI->selectedSlot == -1)
			break;

		inventoryUI->imgSlots[inventoryUI->selectedSlot]->alpha = 1;

		if (inventoryUI->selectingSlot != -1)
		{
			audioPlay(snd_eff_item_drop);
			inventory->move(inventoryUI->selectedSlot, inventoryUI->selectingSlot);
		}
		else if (equipUI->selectingSlot != -1)
		{
			equipUI->equip();
		}
		break;
	}
	}

	if (containPoint(p, inventoryUI->imgBg->rect(inventoryUI->popup->closePoint)))
		return true;

	return false;
}

void showInventoryUI(bool isShow)
{
	if (isShow == inventoryUI->popup->isShow)
		isShow = !isShow;

	if (isShow)
		audioPlay(snd_eff_chest_open);
	else
		audioPlay(snd_eff_chest_close);

	inventoryUI->popup->show(isShow);
}
