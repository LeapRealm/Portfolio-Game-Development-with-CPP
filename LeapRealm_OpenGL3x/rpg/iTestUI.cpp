#include "iTestUI.h"

#include "iEquipUI.h"
#include "iInventoryUI.h"
#include "iTopMenuUI.h"
#include "iUI.h"

iTestUI::iTestUI() : iItemUI(0)
{
}

iTestUI::~iTestUI()
{
}

void iTestUI::setPopup()
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
	iRect rt = rectOfString("테스트창");
	if (rt.size.width > (size.width - StrPadding))
		setStringSize(stringSize * (size.width - StrPadding) / rt.size.width);

	g->drawString(size.width / 2, 47, VCENTER | HCENTER, "테스트창");

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
	popup->methodDrawBefore = drawTestUIBefore;

	delete bg;
	delete close;
	delete g;
}

void iTestUI::paint(float dt)
{
	popup->paint(dt);
}

void iTestUI::paintAfter(float dt)
{

}

void drawTestUIBefore(float dt, iPopup* pop)
{
	testUI->imgBtn->frame = (0 == testUI->popup->selected);
}

iTestUI* testUI;

void loadTestUI()
{
	testUI = new iTestUI();
	testUI->setPopup();
}

void freeTestUI()
{
	delete testUI;
}

void drawTestUI(float dt)
{
	testUI->paint(dt);
}

bool keyTestUI(iKeyState state, iPoint p)
{
	if (testUI->popup->isShow == false)
		return false;
	if (testUI->popup->state != iPopupStateProc)
		return true;

	switch (state)
	{
	case iKeyStateBegan:
	{
		if (containPoint(p, testUI->imgBg->rect(testUI->popup->closePoint)))
		{
			listLayer->removeObject(testLayer);
			listLayer->addObject(testLayer);
		}

		if (containPoint(p, iRectMake(testUI->popup->closePoint.x, testUI->popup->closePoint.y, 300, 75)))
		{
			testUI->isClicked = true;
			selectedPos = testUI->popup->closePoint;
			mousePosition = p;
			break;
		}

		int s = testUI->popup->selected;
		if (s == 0)
		{
			audioPlay(snd_eff_chest_close);
			showTestUI(false);
		}
		break;
	}

	case iKeyStateMoved:
	{
		if (testUI->isClicked)
		{
			selectedPos += (p - mousePosition);
			testUI->popup->closePoint = selectedPos;
			mousePosition = p;
			break;
		}

		int j = -1;
		if (containPoint(p, testUI->imgBtn->rect(testUI->popup->closePoint)))
			j = 0;
		testUI->popup->selected = j;
		break;
	}

	case iKeyStateEnded:
	{
		if (testUI->isClicked)
		{
			testUI->isClicked = false;
			break;
		}
		break;
	}
	}

	if (containPoint(p, testUI->imgBg->rect(testUI->popup->closePoint)))
	{
		hoveredIndex = -1;
		inventoryUI->selectingSlot = -1;
		equipUI->selectingSlot = -1;
		return true;
	}

	return false;
}

void showTestUI(bool isShow)
{
	if (isShow == testUI->popup->isShow)
		isShow = !isShow;

	if (isShow)
		audioPlay(snd_eff_chest_open);
	else
		audioPlay(snd_eff_chest_close);

	testUI->popup->show(isShow);
}
