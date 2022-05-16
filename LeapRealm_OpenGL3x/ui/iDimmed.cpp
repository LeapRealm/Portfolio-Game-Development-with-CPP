#include "iDimmed.h"

#include "iTopMenu.h"

iPopup* dimmed;

void createDimmed()
{
	iPopup* pop = new iPopup();

	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(devSize.width, devSize.height);
	g->init(size);

	setRGBA(0, 0, 0, 0.7f);
	g->fillRect(0, 0, size.width, size.height);

	Texture* tex = g->getTexture();
	iImage* img = new iImage();
	img->addTexture(tex);
	freeTexture(tex);

	pop->addImage(img);

	setRGBA(1, 1, 1, 1);

	pop->style = iPopupStyleAlpha;
	pop->openPoint = iPointZero;
	pop->closePoint = iPointZero;

	dimmed = pop;
	delete g;
}

void freeDimmed()
{
	delete dimmed;
}

void drawDimmed(float dt)
{
	dimmed->paint(dt);
}

void showDimmed(bool isShow)
{
	if (isShow == false)
		topMenuUI->selected = -1;

	dimmed->show(isShow);
}
