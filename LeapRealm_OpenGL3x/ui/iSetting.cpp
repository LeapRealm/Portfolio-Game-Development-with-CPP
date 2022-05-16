#include "iSetting.h"

#include "App.h"
#include "iDimmed.h"
#include "iSound.h"
#include "iTopMenu.h"
#include "iUI.h"

iPopup* settingUI;
iImage** imgSettingBtn;
iImage** imgSliderFill;

float bgmVolume;
float sfxVolume;
bool isFullScreen;

void drawSettingBefore(float dt, iPopup* pop);

float rangeBtn[2]; // min / max

void loadSetting()
{
	iPopup* pop = new iPopup();
	iGraphics* g = new iGraphics();

	igImage* bg = iGraphics::createIgImage("assets/ui/setting_bg.png");

	igImage* btn[7];
	for (int i = 0; i < 7; i++)
		btn[i] = iGraphics::createIgImage("assets/ui/setting_btn_%d.png", i);

	iSize size = iSizeMake(421, 424);
	g->init(size);

	setRGBA(1, 1, 1, 1);
	g->drawIgImage(bg, 0, 0);

	setStringName("assets/fonts/NanumSquareEB.ttf");
	setStringRGBA(0.96f, 0.83f, 0.53f, 1);
	setStringBorder(0);

	float stringSize = 25;
	setStringSize(stringSize);
	iRect rt = rectOfString("설정");
	if (rt.size.width > (size.width - 50))
		setStringSize(stringSize * (size.width - 50) / rt.size.width);
	
	g->drawString(size.width / 2, 37, VCENTER | HCENTER, "설정");

	const char* strOption[3] = { "배경음악", "효과음", "전체화면" };
	setStringName("assets/fonts/NanumSquareB.ttf");
	stringSize = 20;
	setStringSize(stringSize);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(0);
	for (int i = 0; i < 3; i++)
	{
		setStringSize(stringSize);
		rt = rectOfString(strOption[i]);
		if (rt.size.width > (size.width - 20))
			setStringSize(stringSize * (size.width - 20) / rt.size.width);
			
		g->drawString(160, 120 + 60 * i, VCENTER | RIGHT, strOption[i]);

		if (i != 2)
			g->drawIgImage(btn[1], 188, 112 + 60 * i, 0.8f, 0.8f, TOP | LEFT);
	}

	Texture* tex = g->getTexture();
	iImage* img = new iImage();
	img->addTexture(tex);
	freeTexture(tex);

	pop->addImage(img);

	// Yellow Stick [0]
	imgSliderFill = new iImage * [2];
	img = new iImage();
	size = iSizeMake(189 * 0.8f, 10 * 0.8f);
	g->init(size);
	g->drawIgImage(btn[2], 0, 0, 0.78f, 0.78f, TOP | LEFT);

	tex = g->getTexture();
	img->addTexture(tex);
	freeTexture(tex);

	img->position = iPointMake(191, 115);
	pop->addImage(img);
	imgSliderFill[0] = img;

	// Yellow Stick [1]
	img = new iImage();
	g->init(size);
	g->drawIgImage(btn[2], 0, 0, 0.78f, 0.78f, TOP | LEFT);

	tex = g->getTexture();
	img->addTexture(tex);
	freeTexture(tex);

	img->position = iPointMake(191, 175);
	pop->addImage(img);
	imgSliderFill[1] = img;

	// Slider Button [0]
	imgSettingBtn = new iImage * [4];

	img = new iImage();
	size = iSizeMake(30, 30);
	for (int i = 0; i < 2; i++)
	{
		g->init(size);

		if (i == 0)
			setRGBA(1, 1, 1, 1);
		else
			setRGBA(0.78f, 0.78f, 0.78f, 1);

		g->drawIgImage(btn[0], 0, 0, 1, 1, TOP | LEFT);

		tex = g->getTexture();
		img->addTexture(tex);
		freeTexture(tex);
	}

	img->position = iPointMake(185 + 137, 106);
	pop->addImage(img);
	imgSettingBtn[0] = img;

	// Slider Button [1]
	img = img->clone();
	img->position.y += 60;
	pop->addImage(img);
	imgSettingBtn[1] = img;

	rangeBtn[0] = 185;
	rangeBtn[1] = rangeBtn[0] + 137;

	// Check Box
	img = new iImage();
	size = iSizeMake(32 * 0.8f, 35 * 0.8f);
	setRGBA(1, 1, 1, 1);
	for (int i = 0; i < 2; i++)
	{
		g->init(size);

		g->drawIgImage(btn[i + 5], 0, 0, 0.75f, 0.75f, TOP | LEFT);

		tex = g->getTexture();
		img->addTexture(tex);
		freeTexture(tex);
	}

	img->position = iPointMake(260, 226);
	pop->addImage(img);
	imgSettingBtn[2] = img;

	// Button [0]
	const char* strBtn = "닫기";
	setStringName("assets/fonts/NanumSquareB.ttf");
	setStringBorder(0);
	size = iSizeMake(158 * 0.9f, 56 * 0.8f);

	stringSize = 17;
	setStringSize(stringSize);

	img = new iImage();
	for (int j = 0; j < 2; j++)
	{
		g->init(size);

		setStringRGBA(0.09f, 0.09f, 0.09f, 1);

		if (j == 0)
			setRGBA(1, 1, 1, 1);
		else
			setRGBA(0.78f, 0.78f, 0.78f, 1);

		g->drawIgImage(btn[4], 0, 0, 0.9f, 0.79f, TOP | LEFT);

		setStringSize(stringSize);
		rt = rectOfString(strBtn);
		if (rt.size.width > (size.width - 25))
			setStringSize(stringSize * (size.width - 25) / rt.size.width);

		g->drawString(size.width / 2, size.height / 2 - 1, VCENTER | HCENTER, strBtn);

		tex = g->getTexture();
		img->addTexture(tex);
		freeTexture(tex);
	}

	img->position = iPointMake(210 - (158 * 0.9f / 2.0f), 310);
	pop->addImage(img);
	imgSettingBtn[3] = img;

	pop->style = iPopupStyleZoom;
	pop->openPoint = imgTopMenuBtn[4]->center(topMenuUI->closePoint);
	pop->closePoint = iPointMake((devSize.width - 421) / 2, (devSize.height - 424) / 2);
	pop->methodDrawBefore = drawSettingBefore;

	setRGBA(1, 1, 1, 1);
	setStringRGBA(1, 1, 1, 1);

	for (int i = 0; i < 3; i++)
		delete btn[i];
	delete bg;
	settingUI = pop;
	delete g;
}

void freeSetting()
{
	delete settingUI;
	delete imgSettingBtn;
}

void drawSettingBefore(float dt, iPopup* pop)
{
	for (int i = 0; i < 4; i++)
	{
		if(i != 2)
			imgSettingBtn[i]->frame = (i == settingUI->selected);
		else
			imgSettingBtn[i]->alpha = (i == settingUI->selected ? 0.7f : 1.0f);
	}

	for (int i = 0; i < 2; i++)
	{
		float x = imgSettingBtn[i]->position.x;
		imgSliderFill[i]->tex->width = (x - rangeBtn[0]);
	}
}

void drawSetting(float dt)
{
	settingUI->paint(dt);
}

bool settingDrag = false;
iPoint prevPoint;
bool keySetting(iKeyState state, iPoint p)
{
	if (settingUI->isShow == false)
		return false;
	if (settingUI->state != iPopupStateProc)
		return true;

	switch (state)
	{
	case iKeyStateBegan:
	{
		int s = settingUI->selected;
		if (s == -1)
			break;

		if (s == 0)
		{
			settingDrag = true;
			prevPoint = p;
		}
		else if (s == 1)
		{
			settingDrag = true;
			prevPoint = p;
		}
		else if (s == 2)
		{
			imgSettingBtn[2]->frame = (imgSettingBtn[2]->frame + 1) % 2;
			setFullscreen(imgSettingBtn[2]->frame);
		}
		else
		{
			audioPlay(snd_eff_mouse_click);
			showDimmed(false);
			showSetting(false);
		}
			
		break;
	}

	case iKeyStateMoved: 
	{
		if (settingDrag)
		{
			iPoint mp = (p - prevPoint);
			prevPoint = p;

			float& x = imgSettingBtn[settingUI->selected]->position.x;
			x += mp.x;

			if (x < rangeBtn[0])
				x = rangeBtn[0];
			else if (x > rangeBtn[1])
				x = rangeBtn[1];

			float percent = (x - rangeBtn[0]) / (rangeBtn[1] - rangeBtn[0]);

			float vol[2];
			for (int i = 0; i < 2; i++)
			{
				float& px = imgSettingBtn[i]->position.x;
				vol[i] = (px - rangeBtn[0]) / (rangeBtn[1] - rangeBtn[0]);
			}
			audioVolume(vol[0], vol[1], snd_eff_max);
			break;
		}

		int j = -1;
		for (int i = 0; i < 4; i++)
		{
			if (containPoint(p, imgSettingBtn[i]->rect(settingUI->closePoint)))
			{
				j = i;
				break;
			}
		}

		if ((settingUI->selected != j && j != -1) && (j == 3 || j == 4))
			audioPlay(snd_eff_mouse_hover);

		settingUI->selected = j;
		break;
	}

	case iKeyStateEnded: 
	{
		if (settingDrag)
		{
			settingDrag = false;
			settingUI->selected = -1;
		}
		break;
	}
	}

	return true;
}

void showSetting(bool isShow)
{
	if (isShow)
		settingUI->selected = -1;

	settingUI->show(isShow);
}