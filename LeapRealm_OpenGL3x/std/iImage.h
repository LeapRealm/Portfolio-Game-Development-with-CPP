#pragma once

#include "iCommon.h"
#include "iLinkedList.h"
#include "iPoint.h"
#include "iRect.h"

typedef void (*CBAnimation)(void* param);

class iImage
{
public:
	iImage();
	virtual ~iImage();

	static void methodDeleteTexture(void* tex);

	void paint(float dt, iPoint p);
	void addTexture(Texture* tex);

	void startAnimation(CBAnimation m = nullptr, void* p = nullptr);
	void stopAnimation();

	iRect rect(iPoint p = iPointZero, iSize s = iSizeZero);
	iPoint center(iPoint p = iPointZero);
	iImage* clone();
	 
public:
	iLinkedList* texs;

	Texture* tex;
	iPoint position;
	int frame;
	float currAnimDt, animDt;
	bool isAnimation;
	uint8 currRepeatNum, repeatNum;
	CBAnimation method;
	void* param;

	float alpha;
	float degree;
	float scale;
	int reverse, anchor;
};

#define default_animDt 0.18f
