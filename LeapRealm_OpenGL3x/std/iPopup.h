#pragma once

#include "iLinkedList.h"
#include "iPoint.h"
#include "iImage.h"

class iPopup;
typedef void (*MethodPopOpenClose)(iPopup* pop);
typedef void (*MethodPopDraw)(float dt, iPopup* pop);

enum iPopupStyle
{
	iPopupStyleAlpha = 0,
	iPopupStyleMove,
	iPopupStyleAlphaMove,
	iPopupStyleZoom,
	iPopupStyleRotate,
};

enum iPopupState
{
	iPopupStateOpen = 0,
	iPopupStateProc,
	iPopupStateClose
};

class iPopup
{
public:
	iPopup();
	virtual ~iPopup();

	static void methodDeleteImage(void* img);

	void paint(float dt);

	void addImage(iImage* img);

	void show(bool show);
	bool prevShow;

public:
	iLinkedList* imgs;// iImage*

	iPopupStyle style;
	iPopupState state;
	iPoint openPoint, closePoint;
	float currAnimDt, animDt;
	bool isShow;
	int selected;

	MethodPopOpenClose methodOpen, methodClose;
	MethodPopDraw methodDrawBefore, methodDrawAfter;
};

#define default_popDt 0.2f
