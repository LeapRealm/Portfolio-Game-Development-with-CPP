#include "iLogUI.h"

struct iLogMsg
{
	iLogMsg();
	~iLogMsg();

	iString* string;

	MsgAttr attr;
	DWORD time;
};

iLogMsg::iLogMsg()
{
	string = new iString("");
}

iLogMsg::~iLogMsg()
{
	delete string;
}

#define LogMaxCnt 10
#define strSize 17
#define strWidth 400
#define disappearDt 3.0f

iPopup* popLogUI;
iLinkedList* logMsgs;
iStrTex** logTexs;
iImage* imgBg;
iImage* imgBar;

int logOffset = 0;
float barWidth = 10;
float barMaxHeight;

Texture* methodStrLog(const char* str);

void MethodDeleteLogMsg(void* data)
{
	iLogMsg* lm = (iLogMsg*)data;
	delete lm;
}

void drawLogUIBefore(float dt, iPopup* pop);

void loadLogUI()
{
	iPopup* pop = new iPopup();

	logMsgs = new iLinkedList(MethodDeleteLogMsg);
	logTexs = new iStrTex * [LogMaxCnt];

	iGraphics* g = new iGraphics();

	iSize size = iSizeMake(strWidth + 40, (strSize + 5) * LogMaxCnt + 40);
	g->init(size);
	barMaxHeight = size.height - 10;

	// Bg
	setRGBA(0.39f, 0.40f, 0.44f, 0.4f);
	g->fillRect(0, 0, size.width, size.height);

	// Bar Line
	setLineWidth(5);
	setRGBA(1, 1, 1, 1);
	g->drawLine(size.width + 1, 0, size.width + 1, size.height);

	Texture* tex = g->getTexture();
	iImage* img = new iImage();
	img->addTexture(tex);
	freeTexture(tex);
	pop->addImage(img);
	imgBg = img;

	// Log StrTex
	for (int i = 0; i < LogMaxCnt; i++)
	{
		iImage* img = new iImage();
		iStrTex* st = new iStrTex(methodStrLog);
		setStringRGBA(0, 0, 0, 0);
		st->setString("test");
		setStringRGBA(1, 1, 1, 1);
		img->addTexture(st->tex);
		img->position = iPointMake(20, ((strSize + 5) * LogMaxCnt) - ((strSize + 5) * i));
		pop->addImage(img);

		logTexs[i] = st;
	}

	// Bar
	img = new iImage();
	size = iSizeMake(barWidth, imgBg->tex->height);
	for (int i = 0; i < 2; i++)
	{
		g->init(size);

		if (i == 0)
			setRGBA(1, 1, 1, 1);
		else
			setRGBA(0.78f, 0.78f, 0.78f, 1);

		g->fillRect(0, 0, barWidth, imgBg->tex->height);

		tex = g->getTexture();
		img->addTexture(tex);
		freeTexture(tex);
	}
	pop->addImage(img);
	imgBar = img;

	pop->style = iPopupStyleAlpha;
	pop->openPoint = pop->closePoint = iPointMake(15, devSize.height - 280.0f);
	pop->animDt = 0.1f;
	pop->methodDrawBefore = drawLogUIBefore;
	popLogUI = pop;

	delete g;

	addLogMessage(MsgAttrGeneral, "포트폴리오 프로젝트에 오신 것을 환영합니다.");
}

void freeLogUI()
{
	delete logMsgs;

	for (int i = 0; i < LogMaxCnt; i++)
		delete logTexs[i];
	delete logTexs;

	delete popLogUI;
}

Texture* methodStrLog(const char* str)
{
	setStringName("assets/fonts/NanumSquareB.ttf");
	setStringSize(strSize);
	setStringBorder(0);

	iGraphics* g = new iGraphics();
	iSize size = iSizeMake(strWidth, strSize);
	g->init(size);
	g->drawString(0, 0, TOP | LEFT, str);

	Texture* tex = g->getTexture();
	delete g;

	return tex;
}

void drawLogUIBefore(float dt, iPopup* pop)
{
	// bar frame
	imgBar->frame = (0 == popLogUI->selected);

	// bar position and size
	float barHeight;
	if (logMsgs->count <= LogMaxCnt)
		barHeight = barMaxHeight;
	else
		barHeight = barMaxHeight * (1.0f * LogMaxCnt / logMsgs->count);

	float barOffset;
	if (logMsgs->count <= LogMaxCnt)
	{
		barOffset = 5;
	}
	else
	{
		int logOffsetMax = logMsgs->count - LogMaxCnt;
		barOffset = linear(imgBg->tex->height - 5 - barHeight, 5, 1.0f * logOffset / logOffsetMax);
	}

	for (int i = 0; i < 2; i++)
	{
		Texture* tex = (Texture*)imgBar->texs->getObjectByIndex(i);
		tex->width = barWidth;
		tex->height = barHeight;
	}
	imgBar->position = iPointMake(imgBg->tex->width - barWidth / 2, barOffset);

	// logStr
	DWORD now = GetTickCount();

	for (int i = logOffset; i < min(logOffset + LogMaxCnt, logMsgs->count); i++)
	{
		iLogMsg* lm = (iLogMsg*)logMsgs->getObjectByIndex(i);

		DWORD timeDiff = now - lm->time;
		timeDiff /= 1000;

		switch (lm->attr)
		{
		case MsgAttrGeneral:	setStringRGBA(1.0f, 1.0f, 1.0f, 1.0f);	break;
		case MsgAttrNotice:		setStringRGBA(1.0f, 0.5f, 0.0f, 1.0f);	break;
		case MsgAttrWarning:	setStringRGBA(1.0f, 0.8f, 0.0f, 1.0f);	break;
		}
		if (timeDiff < 60)
		{
			logTexs[i - logOffset]->setString("[방금전] %s", lm->string->str);
		}
		else
		{
			timeDiff /= 60;
			logTexs[i - logOffset]->setString("[%d분전] %s", timeDiff, lm->string->str);
		}
		setStringRGBA(1, 1, 1, 1);
	}
}

void drawLogUI(float dt)
{
	popLogUI->paint(dt);
}

static bool logDrag = false;
static iPoint prevPoint = iPointZero;
static float mDelta = 0.0f;
bool keyLogUI(iKeyState state, iPoint p)
{
	if (popLogUI->isShow == false)
		return false;
	if (popLogUI->state != iPopupStateProc)
		return true;

	switch (state)
	{
	case iKeyStateBegan:
	{
		int s = popLogUI->selected;
		if (s == -1)
			break;

		if (s == 0)
		{
			logDrag = true;
			prevPoint = p;
		}

		break;
	}

	case iKeyStateMoved: 
	{
		if (logDrag)
		{
			mDelta += (p.y - prevPoint.y);
			prevPoint = p;

			float& y = imgBar->position.y;
			y += mDelta;

			float barHeight;
			if (logMsgs->count <= LogMaxCnt)
				barHeight = barMaxHeight;
			else
				barHeight = barMaxHeight * (1.0f * LogMaxCnt / logMsgs->count);

			if (y < 5)
				y = 5;
			else if (y > imgBg->tex->height - 5 - barHeight)
				y = imgBg->tex->height - 5 - barHeight;

			// offset : logOffsetMax ~ 0
			// posY   : 5 ~ imgBg->tex->height - 5 - barHeight
			int logOffsetMax = logMsgs->count - LogMaxCnt;
			if (logOffsetMax < 0)
				logOffsetMax = 0;

			float y1 = logOffsetMax, y2 = 0;
			float x1 = 5, x2 = imgBg->tex->height - 5 - barHeight;

			float offsetF = y1 + ((y - x1) / (x2 - x1)) * (y2 - y1);
			int OffsetI = (int)(floorf(offsetF + 0.5f));
			if (OffsetI < 0)
				OffsetI = 0;
			else if (OffsetI > logOffsetMax)
				OffsetI = logOffsetMax;

			if (logOffset != OffsetI)
				mDelta = 0;
			logOffset = OffsetI;

			break;
		}

		int j = -1;
		if (containPoint(p, imgBar->rect(popLogUI->closePoint)))
			j = 0;
		popLogUI->selected = j;
		break;
	}

	case iKeyStateEnded: 
	{
		if (logDrag)
		{
			mDelta = 0;
			logDrag = false;
			popLogUI->selected = -1;
		}
		break;
	}

	case iKeyStateWheelBtnUp:
	{
		if (containPoint(p, imgBg->rect(popLogUI->closePoint)) == false)
			return false;

		logOffset++;

		if (logMsgs->count > LogMaxCnt)
		{
			int logOffsetMax = logMsgs->count - LogMaxCnt;
			if (logOffset > logOffsetMax)
				logOffset = logOffsetMax;
		}
		else
		{
			logOffset = 0;
		}
		break;
	}

	case iKeyStateWheelBtnDown: 
	{
		if (containPoint(p, imgBg->rect(popLogUI->closePoint)) == false)
			return false;

		logOffset--;

		if (logOffset < 0)
			logOffset = 0;
		break;
	}
	}

	return false;
}

void showLogUI(bool isShow)
{
	popLogUI->show(isShow);
}

void addLogMessage(MsgAttr ma, const char* szFormat, ...)
{
	char szText[512];
	va_start_end(szFormat, szText);

	iLogMsg* lm = new iLogMsg();
	lm->string->copy(szText);
	lm->attr = ma;
	lm->time = GetTickCount();
	logMsgs->addObject(0, lm);
}
