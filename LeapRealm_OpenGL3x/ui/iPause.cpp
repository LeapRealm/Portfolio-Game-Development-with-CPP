#include "iPause.h"

#include "App.h"
#include "iDimmed.h"
#include "iTopMenu.h"

iPopup* pauseUI;
iImage** imgPauseBtn;

void drawPauseBefore(float dt, iPopup* pop);

void loadPause()
{
	iPopup* pop = new iPopup();
	iGraphics* g = new iGraphics();

	igImage* bg = g->createIgImage("assets/ui/pause_bg.png");

	iSize size = iSizeMake(307, 380);
	g->init(size);

	setRGBA(1, 1, 1, 1);
	g->drawIgImage(bg, 0, 0);

	setStringName("assets/fonts/NanumSquareEB.ttf");
	setStringRGBA(0.96f, 0.83f, 0.53f, 1);
	setStringBorder(0);

	float stringSize = 25;
	setStringSize(stringSize);
	iRect rt = rectOfString("일시정지");
	if (rt.size.width > (size.width - 50))
		setStringSize(stringSize * (size.width - 50) / rt.size.width);
		
	g->drawString(size.width / 2, 33, VCENTER | HCENTER, "일시정지");

	Texture* tex = g->getTexture();
	iImage* img = new iImage();
	img->addTexture(tex);
	freeTexture(tex);

	pop->addImage(img);

	const char* str[3] = { "게임재개", "메인메뉴", "게임종료" };
	setStringName("assets/fonts/NanumSquareB.ttf");
	setStringBorder(0);
	size = iSizeMake(238 * 0.8f, 84 * 0.6f);

	stringSize = 20;
	setStringSize(stringSize);

	igImage* btn = g->createIgImage("assets/ui/pause_btn.png");

	imgPauseBtn = new iImage * [3];
	for (int i = 0; i < 3; i++)
	{
		img = new iImage();
		for (int j = 0; j < 2; j++)
		{
			g->init(size);

			if (j == 0)
				setRGBA(1, 1, 1, 1);
			else
				setRGBA(0.78f, 0.78f, 0.78f, 1);

			setStringRGBA(0.09f, 0.09f, 0.09f, 1);

			g->drawIgImage(btn, 0, 0, 0.8f, 0.6f, TOP | LEFT);

			setStringSize(stringSize);
			rt = rectOfString(str[i]);
			if (rt.size.width > (size.width - 25))
				setStringSize(stringSize * (size.width - 25) / rt.size.width);
				
			g->drawString(size.width / 2, size.height / 2 - 1, VCENTER | HCENTER, str[i]);

			tex = g->getTexture();
			img->addTexture(tex);
			freeTexture(tex);
		}

		img->position = iPointMake(58, 100 + 85 * i);
		pop->addImage(img);
		imgPauseBtn[i] = img;
	}

	pop->style = iPopupStyleZoom;
	pop->openPoint = imgTopMenuBtn[0]->center(topMenuUI->closePoint);
	pop->closePoint = iPointMake((devSize.width - 307) / 2, (devSize.height - 380) / 2);
	pop->methodDrawBefore = drawPauseBefore;

	setRGBA(1, 1, 1, 1);
	setStringRGBA(1, 1, 1, 1);

	delete btn;
	delete bg;
	pauseUI = pop;
	delete g;
}

void freePause()
{
	delete pauseUI;
	delete imgPauseBtn;
}

void drawPauseBefore(float dt, iPopup* pop)
{
	for (int i = 0; i < 3; i++)
		imgPauseBtn[i]->frame = (i == pauseUI->selected);
}

void drawPause(float dt)
{
	pauseUI->paint(dt);
}

bool keyPause(iKeyState state, iPoint p)
{
	if (pauseUI->isShow == false)
		return false;
	if (pauseUI->state != iPopupStateProc)
		return true;

	int j = -1;

	switch (state)
	{
	case iKeyStateBegan:
	{
		int s = pauseUI->selected;
		if (s == -1)
			break;

		if (s == 0)
		{
			audioPlay(snd_eff_mouse_click);
			topMenuUI->selected = -1;
			pauseUI->selected = -1;
			showDimmed(false);
			showPause(false);
		}
		else if (s == 1)
		{
			audioPlay(snd_eff_mouse_click);
		}
		else
		{
			audioPlay(snd_eff_mouse_click);
			runApp = false;
		}

		break;
	}

	case iKeyStateMoved: 
	{
		for (int i = 0; i < 3; i++)
		{
			if (containPoint(p, imgPauseBtn[i]->rect(pauseUI->closePoint)))
			{
				j = i;
				break;
			}
		}

		if (pauseUI->selected != j && j != -1)
			audioPlay(snd_eff_mouse_hover);

		pauseUI->selected = j;
		break;
	}

	case iKeyStateEnded: 
	{

		break;
	}
	}

	return true;
}

void showPause(bool isShow)
{
	if (isShow)
		pauseUI->selected = -1;

	pauseUI->show(isShow);
}
