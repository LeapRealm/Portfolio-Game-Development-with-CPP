#include "iCaptionUI.h"

#include "iItem.h"
#include "iItemUI.h"

iPopup* popCaptionUI;
iStrTex* strCaption;
Texture* methodStrCaption(const char* str);

void loadCaptionUI()
{
	iPopup* pop = new iPopup();

	iImage* img = new iImage();

	iStrTex* st = new iStrTex(methodStrCaption);
	st->setString("Item_Info");
	img->addTexture(st->tex);
	strCaption = st;

	pop->addImage(img);

	pop->style = iPopupStyleAlpha;
	pop->animDt = 0.1f;
	popCaptionUI = pop;
}

void freeCaptionUI()
{
	delete strCaption;
	delete popCaptionUI;
}

Texture* methodStrCaption(const char* str)
{
	int lineNum;
	char** line = iString::getStringLine(str, lineNum);

	setStringSize(15);
	setStringRGBA(1, 1, 1, 1);
	setStringBorder(0);

	int maxWidth = 0;
	for(int i = 0; i < lineNum - 1; i++)
	{
		iRect rt = rectOfString(line[i]);
		if (rt.size.width > maxWidth)
			maxWidth = rt.size.width;
	}

	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(maxWidth + 20, (getStringSize() + 5) * (lineNum - 1) + 20);
	g->init(size);

	switch (atoi(line[lineNum - 1]))
	{
	case iItemGradeCommon:		setRGBA(0.08f, 0.18f, 0.13f, 0.8f);	break;
	case iItemGradeRare:		setRGBA(0.13f, 0.05f, 0.20f, 0.8f);	break;
	case iItemGradeLegendary:	setRGBA(0.18f, 0.12f, 0.07f, 0.8f);	break;
	}
	g->fillRect(0, 0, size.width, size.width, 4);
	setLineWidth(1);
	setRGBA(1, 1, 1, 0.8f);
	g->drawRect(2, 2, size.width - 4, size.height - 4);

	for (int i = 0; i < lineNum - 1; i++)
	{
		setStringRGBA(1, 1, 1, 1);
		g->drawString(10, 20 + (getStringSize() + 5) * i, VCENTER | LEFT, line[i]);
	}

	iString::freeStringLine(line, lineNum);
	Texture* tex = g->getTexture();
	delete g;

	return tex;
}

void drawCaptionUI(float dt)
{
	if (hoveredIndex == -1)
		showCaptionUI(dt, -1, mp);

	popCaptionUI->paint(dt);
}

int prevItemIdx = -1;
float showDt = 0;
iPoint prevP = iPointZero;
void showCaptionUI(float dt, int itemIndex, iPoint p)
{
	if (itemIndex == -1 || (itemIndex != prevItemIdx && p != prevP))
	{
		popCaptionUI->show(false);

		showDt = 0;
		prevP = p;
		return;
	}

	if (showDt < 0.25f)
	{
		showDt += dt;
		return;
	}

	if (itemIndex != prevItemIdx)
	{
		iItem* it = items[itemIndex];

		char grade[10]{};
		char kind[10]{};

		switch (it->grade)
		{
		case iItemGradeCommon:		strcpy(grade, "일반");	break;
		case iItemGradeRare:		strcpy(grade, "희귀");	break;
		case iItemGradeLegendary:	strcpy(grade, "전설");	break;
		}

		switch (it->itemKind)
		{
		case iItemKindEquip:		strcpy(kind, "장비");	break;
		case iItemKindConsume:		strcpy(kind, "소비");	break;
		}

		iString* s = new iString("이름 : %s\n설명 : %s\n등급 : %s\n종류 : %s\n가격 : %d\n최대 스택 개수 : %d\n%d",
			it->name->str, it->desc->str, grade, kind, it->price, it->stackCntMax, it->grade);
		strCaption->setString(s->str);
		delete s;
	}

	iPoint pos = p;
	if (p.x > devSize.width - strCaption->tex->width - 35)
		pos.x -= (strCaption->tex->width + 35);
	if (p.y > devSize.height - strCaption->tex->height)
		pos.y -= (strCaption->tex->height);

	popCaptionUI->openPoint = popCaptionUI->closePoint = iPointMake(pos.x + 35, pos.y);

	popCaptionUI->show(true);

	prevItemIdx = itemIndex;
	prevP = p;
}
