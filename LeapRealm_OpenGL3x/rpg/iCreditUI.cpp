#include "iCreditUI.h"

#include "iEquipUI.h"
#include "iInventoryUI.h"
#include "iTopMenuUI.h"
#include "iUI.h"

iCreditUI::iCreditUI() : iItemUI(0)
{
}

iCreditUI::~iCreditUI()
{
}

void iCreditUI::setPopup()
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
	iRect rt = rectOfString("프로젝트 설명");
	if (rt.size.width > (size.width - StrPadding))
		setStringSize(stringSize * (size.width - StrPadding) / rt.size.width);

	g->drawString(size.width / 2, 47, VCENTER | HCENTER, "프로젝트 설명");

	setStringName("assets/fonts/NanumSquareEB.ttf");
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(0);
	setStringSize(20);

	g->drawString(size.width / 2, 135, VCENTER | HCENTER, "인벤토리 & 장비창 및 로그 기능 구현");

	g->drawString(100, 210, VCENTER | HCENTER, "제작자");
	g->drawString(100, 260, VCENTER | HCENTER, "사용기술");
	g->drawString(100, 335, VCENTER | HCENTER, "사용언어");
	g->drawString(100, 385, VCENTER | HCENTER, "제작기간");

	g->drawString(200, 210, VCENTER | LEFT, "김진우");
	g->drawString(200, 260, VCENTER | LEFT, "OpenGL 3.2");
	g->drawString(200, 285, VCENTER | LEFT, "OpenAL 1.1");
	g->drawString(200, 335, VCENTER | LEFT, "C++");
	g->drawString(200, 385, VCENTER | LEFT, "약 3개월");

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

	popup->style = iPopupStyleAlpha;
	popup->openPoint = imgTopMenuBtn[3]->center(topMenuUI->closePoint);
	popup->closePoint = iPointMake((devSize.width - 380) - 50, (devSize.height - 320) / 3);
	popup->methodDrawBefore = drawCreditUIBefore;

	delete bg;
	delete close;
	delete g;
}

void iCreditUI::paint(float dt)
{
	popup->paint(dt);
}

void iCreditUI::paintAfter(float dt)
{

}

void drawCreditUIBefore(float dt, iPopup* pop)
{
	creditUI->imgBtn->frame = (0 == creditUI->popup->selected);
}

iCreditUI* creditUI;

void loadCreditUI()
{
	creditUI = new iCreditUI();
	creditUI->setPopup();
}

void freeCreditUI()
{
	delete creditUI;
}

void drawCreditUI(float dt)
{
	creditUI->paint(dt);
}

bool keyCreditUI(iKeyState state, iPoint p)
{
	if (creditUI->popup->isShow == false)
		return false;
	if (creditUI->popup->state != iPopupStateProc)
		return true;

	switch (state)
	{
	case iKeyStateBegan:
	{
		if (containPoint(p, creditUI->imgBg->rect(creditUI->popup->closePoint)))
		{
			listLayer->removeObject(testLayer);
			listLayer->addObject(testLayer);
		}

		if (containPoint(p, iRectMake(creditUI->popup->closePoint.x, creditUI->popup->closePoint.y, 300, 75)))
		{
			creditUI->isClicked = true;
			selectedPos = creditUI->popup->closePoint;
			mousePosition = p;
			break;
		}

		int s = creditUI->popup->selected;
		if (s == 0)
		{
			audioPlay(snd_eff_chest_close);
			showCreditUI(false);
		}
		break;
	}

	case iKeyStateMoved:
	{
		if (creditUI->isClicked)
		{
			selectedPos += (p - mousePosition);
			creditUI->popup->closePoint = selectedPos;
			mousePosition = p;
			break;
		}

		int j = -1;
		if (containPoint(p, creditUI->imgBtn->rect(creditUI->popup->closePoint)))
			j = 0;
		creditUI->popup->selected = j;
		break;
	}

	case iKeyStateEnded:
	{
		if (creditUI->isClicked)
		{
			creditUI->isClicked = false;
			break;
		}
		break;
	}
	}

	if (containPoint(p, creditUI->imgBg->rect(creditUI->popup->closePoint)))
	{
		hoveredIndex = -1;
		inventoryUI->selectingSlot = -1;
		equipUI->selectingSlot = -1;
		return true;
	}

	return false;
}

void showCreditUI(bool isShow)
{
	if (isShow == creditUI->popup->isShow)
		isShow = !isShow;

	if (isShow)
		audioPlay(snd_eff_chest_open);
	else
		audioPlay(snd_eff_chest_close);

	creditUI->popup->show(isShow);
}
