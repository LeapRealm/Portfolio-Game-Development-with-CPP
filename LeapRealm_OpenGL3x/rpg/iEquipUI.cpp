#include "iEquipUI.h"

#include "iCaptionUI.h"
#include "iInventory.h"
#include "iInventoryUI.h"
#include "iLogUI.h"
#include "iTopMenuUI.h"
#include "iUI.h"

iEquipUI::iEquipUI(): iItemUI(EquipSlotMaxCnt)
{
	for (int i = 0; i < slotMaxCnt; i++)
		slotIndexes[i] = -1;

	imgSlots = new iImage * [slotMaxCnt];

	strSlots = new iStrTex * * [slotMaxCnt];
	for (int i = 0; i < slotMaxCnt; i++)
		strSlots[i] = new iStrTex * [2];
}

iEquipUI::~iEquipUI()
{
	delete imgSlots;

	for (int i = 0; i < slotMaxCnt; i++)
	{
		for (int j = 0; j < 2; j++)
			delete strSlots[i][j];
		delete strSlots[i];
	}
	delete strSlots;
}

void iEquipUI::setPopup()
{
	iGraphics* g = new iGraphics();

	igImage* bg = iGraphics::createIgImage("assets/ui/equip_bg.png");
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
	iRect rt = rectOfString("장비창");
	if (rt.size.width > (size.width - StrPadding))
		setStringSize(stringSize * (size.width - StrPadding) / rt.size.width);

	g->drawString(size.width / 2, 47, VCENTER | HCENTER, "장비창");

	// inven slot layout lines
	setRGBA(0.47f, 0.42f, 0.46f, 1);
	int offX = (size.width - EquipItemW * EquipItemX) / 2;
	int offY = 150;
	for (int i = 0; i < EquipItemX + 1; i++)
		g->drawLine(offX + EquipItemW * i, offY, offX + EquipItemW * i, offY + EquipItemH * EquipItemY);
	for (int i = 0; i < EquipItemY + 1; i++)
		g->drawLine(offX, offY + EquipItemH * i, offX + EquipItemW * EquipItemX, offY + EquipItemH * i);

	// slot bg
	setRGBA(1, 1, 1, 0.3f);
	for (int i = 0; i < slotMaxCnt; i++)
	{
		igImage* img = iGraphics::createIgImage("assets/ui/equip_icon_%d.png", i);

		g->drawIgImage(img, offX + EquipItemW * (i % EquipItemX),
							offY + EquipItemH * (i / EquipItemX),
							1, 1, TOP | LEFT);
		delete img;
	}

	Texture* tex = g->getTexture();
	iImage* img = new iImage();
	img->addTexture(tex);
	freeTexture(tex);

	popup->addImage(img);
	imgBg = img;

	// close btn
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
	imgBtn = img;

	// slots
	for (int i = 0; i < slotMaxCnt; i++)
	{
		img = new iImage();

		if (slotIndexes[i] == -1)
		{
			for (int j = 0; j < 2; j++)
			{
				iStrTex* st = new iStrTex(methodStrEquipSlot);
				st->setString("%d/%d/%d/%d", 0, 0, 0, j);

				img->addTexture(st->tex);
				strSlots[i][j] = st;
			}
		}
		else
		{
			iSlot* sl = &inventory->slots[slotIndexes[i]];

			for (int j = 0; j < 2; j++)
			{
				iStrTex* st = new iStrTex(methodStrEquipSlot);
				st->setString("%d/%d/%d/%d", sl->itemIndex, sl->isEquipped, sl->itemCount, j);

				img->addTexture(st->tex);
				strSlots[i][j] = st;
			}
		}

		img->position = iPointMake(offX + EquipItemW * (i % EquipItemX),
								   offY + EquipItemH * (i / EquipItemX));
		popup->addImage(img);
		imgSlots[i] = img;
	}

	popup->style = iPopupStyleAlpha;
	popup->openPoint = imgTopMenuBtn[1]->center(topMenuUI->closePoint);
	popup->closePoint = iPointMake(50, (devSize.height - 320) / 3);
	popup->methodDrawBefore = drawEquipUIBefore;

	delete bg;
	delete close;
	delete g;
}

void iEquipUI::equip()
{
	int slotIndex = inventoryUI->slotIndexes[inventoryUI->selectedSlot];
	iItem* it = items[inventory->slots[slotIndex].itemIndex];
	if (it->itemKind != iItemKindEquip)
		return;

	iEquipItem* ei = (iEquipItem*)it;
	if ((int)(ei->equipKind) != selectingSlot)
		return;

	if (slotIndex == slotIndexes[selectingSlot])
		return;

	int* si = &slotIndexes[selectingSlot];

	if (*si != -1)
		inventory->slots[*si].isEquipped = false;

	iEquipKind ek = ((iEquipItem*)it)->equipKind;

	switch (ek)
	{
	case iEquipKindSword:
		audioPlay(snd_eff_sword_equip);
		break;

	case iEquipKindShield:
		audioPlay(snd_eff_shield_equip);
		break;

	case iEquipKindHelmet:
	case iEquipKindChest:
		audioPlay(snd_eff_cloth_equip);
		break;
	}

	if (*si == -1)
		addLogMessage(MsgAttrNotice, "'%s' 장착했습니다.", it->name->str);
	else
		addLogMessage(MsgAttrWarning, "'%s' 탈착 후 '%s' 장착했습니다.", items[inventory->slots[*si].itemIndex]->name->str, it->name->str);

	*si = slotIndex;
	inventory->slots[*si].isEquipped = true;
}

void iEquipUI::unequip()
{
	iSlot* sl = &inventory->slots[slotIndexes[selectedSlot]];
	iItem* it = items[sl->itemIndex];
	iEquipKind ek = ((iEquipItem*)it)->equipKind;

	switch (ek)
	{
	case iEquipKindSword: 
		audioPlay(snd_eff_sword_unequip);
		break;

	case iEquipKindShield: 
		audioPlay(snd_eff_shield_unequip);
		break;

	case iEquipKindHelmet:
	case iEquipKindChest:
		audioPlay(snd_eff_cloth_unequip);
		break;
	}

	sl->isEquipped = false;
	slotIndexes[selectedSlot] = -1;

	addLogMessage(MsgAttrGeneral, "'%s' 탈착했습니다.", it->name->str);
}

void iEquipUI::paint(float dt)
{
	popup->paint(dt);
}

void iEquipUI::paintAfter(float dt)
{
	if (selectedSlot == -1)
		return;

	Texture* tex = imgSlots[selectedSlot]->tex;
	drawTexture(tex, selectedPos, TOP | LEFT);
}

Texture* methodStrEquipSlot(const char* str)
{
	int lineNum;
	char** line = iString::getStringLine(str, lineNum, '/');
	int itemIndex = atoi(line[0]);
	int isEquipped = atoi(line[1]);
	int itemCount = atoi(line[2]);
	int j = atoi(line[3]);
	iString::freeStringLine(line, lineNum);

	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(EquipItemW, EquipItemH);
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
		g->drawIgImage(items[itemIndex]->image, 0, 0, 1, 1, TOP | LEFT);
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

void drawEquipUIBefore(float dt, iPopup* pop)
{
	if (hoveredIndex != -1 && isInventory == false)
		showCaptionUI(dt, inventory->slots[equipUI->slotIndexes[hoveredIndex]].itemIndex, mp);

	for (int i = 0; i < equipUI->slotMaxCnt; i++)
	{
		if (equipUI->slotIndexes[i] != -1)
		{
			iSlot* sl = &inventory->slots[equipUI->slotIndexes[i]];
			for (int j = 0; j < 2; j++)
				equipUI->strSlots[i][j]->setString("%d/%d/%d/%d", 
					sl->itemIndex, sl->isEquipped, sl->itemCount, j);
		}
		else
		{
			for (int j = 0; j < 2; j++)
				equipUI->strSlots[i][j]->setString("%d/%d/%d/%d", 0, 0, 0, j);
		}
		equipUI->imgSlots[i]->frame = (i == equipUI->selectedSlot || 
									   i == equipUI->selectingSlot);
	}

	equipUI->imgBtn->frame = (0 == equipUI->popup->selected);
}

iEquipUI* equipUI;

void loadEquipUI()
{
	equipUI = new iEquipUI();
	equipUI->setPopup();
}

void freeEquipUI()
{
	delete equipUI;
}

void drawEquipUI(float dt)
{
	equipUI->paint(dt);
}

bool keyEquipUI(iKeyState state, iPoint p)
{
	if (equipUI->popup->isShow == false)
		return false;
	if (equipUI->popup->state != iPopupStateProc)
		return true;

	switch (state)
	{
	case iKeyStateBegan:
	{
		if (containPoint(p, equipUI->imgBg->rect(equipUI->popup->closePoint)))
		{
			listLayer->removeObject(equipLayer);
			listLayer->addObject(equipLayer);
		}

		if (containPoint(p, iRectMake(equipUI->popup->closePoint.x, equipUI->popup->closePoint.y, 300, 75)))
		{
			equipUI->isClicked = true;
			selectedPos = equipUI->popup->closePoint;
			mousePosition = p;
			break;
		}

		for (int i = 0; i < equipUI->slotMaxCnt; i++)
		{
			if (equipUI->slotIndexes[i] != -1)
			{
				iSlot* is = &inventory->slots[equipUI->slotIndexes[i]];
				if (is->itemCount != 0 && containPoint(p, equipUI->imgSlots[i]->rect(equipUI->popup->closePoint)))
				{
					equipUI->selectedSlot = i;
					selectedPos = equipUI->imgSlots[equipUI->selectedSlot]->position + equipUI->popup->closePoint;
					mousePosition = p;
					equipUI->imgSlots[equipUI->selectedSlot]->alpha = 0;
					break;
				}
			}
		}

		int s = equipUI->popup->selected;
		if (s == 0)
		{
			audioPlay(snd_eff_chest_close);
			showEquipUI(false);
		}
		break;
	}
		
	case iKeyStateMoved: 
	{
		if (equipUI->isClicked)
		{
			selectedPos += (p - mousePosition);
			equipUI->popup->closePoint = selectedPos;
			mousePosition = p;
			break;
		}

		int j = -1;
		if (inventoryUI->selectedSlot != -1)
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

		if (equipUI->selectedSlot == -1)
		{
			j = -1;
			if (containPoint(p, equipUI->imgBtn->rect(equipUI->popup->closePoint)))
				j = 0;

			equipUI->popup->selected = j;
		}
		else
		{
			selectedPos += (p - mousePosition);
			mousePosition = p;
			
		}

		j = -1;
		if (equipUI->selectedSlot == -1 && equipUI->selectingSlot == -1)
		{
			for (int i = 0; i < equipUI->slotMaxCnt; i++)
			{
				if (containPoint(p, equipUI->imgSlots[i]->rect(equipUI->popup->closePoint)))
				{
					if (equipUI->slotIndexes[i] != -1)
					{
						isInventory = false;
						j = i;
						break;
					}
				}
			}
		}
		hoveredIndex = j;
		mp = p;
		break;
	}
	
	case iKeyStateEnded: 
	{
		if (equipUI->isClicked)
		{
			equipUI->isClicked = false;
			break;
		}

		if (equipUI->selectedSlot == -1)
			break;

		equipUI->imgSlots[equipUI->selectedSlot]->alpha = 1;

		if (inventoryUI->selectingSlot == -1)
			break;

		equipUI->unequip();
		break; 
	}
	case iKeyStateDBCLK:
	{
		int j = -1;
		for (int i = 0; i < equipUI->slotMaxCnt; i++)
		{
			if (containPoint(p, equipUI->imgSlots[i]->rect(equipUI->popup->closePoint)))
			{
				j = i;
				break;
			}
		}

		if (j != -1)
		{
			int slIdx = equipUI->slotIndexes[j];

			if (slIdx != -1)
			{
				hoveredIndex = -1;
				equipUI->selectedSlot = j;
				equipUI->unequip();
			}
		}
		break;
	}
	}

	if (containPoint(p, equipUI->imgBg->rect(equipUI->popup->closePoint)))
	{
		inventoryUI->selectingSlot = -1;
		return true;
	}

	return false;
}

void showEquipUI(bool isShow)
{
	if (isShow == equipUI->popup->isShow)
		isShow = !isShow;

	if (isShow)
		audioPlay(snd_eff_chest_open);
	else
		audioPlay(snd_eff_chest_close);

	equipUI->popup->show(isShow);
}
