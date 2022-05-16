#pragma once

#include "iSize.h"
#include "iPoint.h"

struct iRect
{
	iPoint origin;
	iSize size;

	iRect();
	iRect(const iRect& r);
	iRect(float x, float y, float width, float height);

	iRect& operator=(const iRect& r);
	iRect& operator=(const iPoint& p);
	iRect& operator=(const iSize& s);

	iRect operator-(const iPoint& p);

	iRect& operator+=(const iPoint& p);
	iRect& operator+=(const iSize& s);
};

iRect iRectMake(float x, float y, float width, float height);

bool containPoint(iPoint p, iRect rt);
bool containRect(iRect src, iRect dst);
 