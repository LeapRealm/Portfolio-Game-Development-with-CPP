#include "iQuickUI.h"

#include "iTopMenu.h"
#include "iUI.h"

iQuickUI::iQuickUI() : iItemUI(0)
{
}

iQuickUI::~iQuickUI()
{
}

void iQuickUI::setPopup()
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
	popup->methodDrawBefore = drawQuickUIBefore;

	delete bg;
	delete close;
	delete g;
}

void iQuickUI::paint(float dt)
{
	popup->paint(dt);
}

void iQuickUI::paintAfter(float dt)
{

}

void drawQuickUIBefore(float dt, iPopup* pop)
{
	quickUI->imgBtn->frame = (0 == quickUI->popup->selected);
}

iQuickUI* quickUI;

void loadQuickUI()
{
	quickUI = new iQuickUI();
	quickUI->setPopup();
}

void freeQuickUI()
{
	delete quickUI;
}

void drawQuickUI(float dt)
{
	quickUI->paint(dt);
}

bool keyQuickUI(iKeyState state, iPoint p)
{
	if (quickUI->popup->isShow == false)
		return false;
	if (quickUI->popup->state != iPopupStateProc)
		return true;

	switch (state)
	{
	case iKeyStateBegan:
	{
		if (containPoint(p, quickUI->imgBg->rect(quickUI->popup->closePoint)))
		{
			listLayer->removeObject(quickLayer);
			listLayer->addObject(quickLayer);
		}

		if (containPoint(p, iRectMake(quickUI->popup->closePoint.x, quickUI->popup->closePoint.y, 300, 75)))
		{
			quickUI->isClicked = true;
			selectedPos = quickUI->popup->closePoint;
			mousePosition = p;
			break;
		}

		int s = quickUI->popup->selected;
		if (s == 0)
		{
			audioPlay(snd_eff_chest_close);
			showQuickUI(false);
		}
		break;
	}

	case iKeyStateMoved:
	{
		if (quickUI->isClicked)
		{
			selectedPos += (p - mousePosition);
			quickUI->popup->closePoint = selectedPos;
			mousePosition = p;
			break;
		}

		int j = -1;
		if (containPoint(p, quickUI->imgBtn->rect(quickUI->popup->closePoint)))
			j = 0;
		quickUI->popup->selected = j;
		break;
	}

	case iKeyStateEnded:
	{
		if (quickUI->isClicked)
		{
			quickUI->isClicked = false;
			break;
		}
		break;
	}
	}

	if (containPoint(p, quickUI->imgBg->rect(quickUI->popup->closePoint)))
		return true;

	return false;
}

void showQuickUI(bool isShow)
{
	if (isShow == quickUI->popup->isShow)
		isShow = !isShow;

	if (isShow)
		audioPlay(snd_eff_chest_open);
	else
		audioPlay(snd_eff_chest_close);

	quickUI->popup->show(isShow);
}
