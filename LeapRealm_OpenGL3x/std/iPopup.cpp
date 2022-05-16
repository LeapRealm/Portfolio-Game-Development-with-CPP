#include "iPopup.h"

#include "iGraphics.h"
#include "iStd.h"

static Texture* texPop = nullptr;

iPopup::iPopup()
{
	imgs = new iLinkedList(methodDeleteImage);
	style = iPopupStyleAlpha;
	state = iPopupStateClose;
	openPoint = closePoint = iPointZero;
	currAnimDt = 0.0f;
	animDt = default_popDt;
	isShow = false;
	methodOpen = nullptr;
	methodClose = nullptr;
	methodDrawBefore = nullptr;
	methodDrawAfter = nullptr;
	selected = -1;
	prevShow = false;

	if (texPop == nullptr)
		texPop = createTexture(devSize.width, devSize.height);
}

iPopup::~iPopup()
{
	delete imgs;
}

void iPopup::methodDeleteImage(void* img)
{
	iImage* i = (iImage*)img;
	delete i;
}
 
void iPopup::paint(float dt)
{
	if (isShow == false)
		return;

	iPoint p = closePoint;
	float scaleX = 1.0f, scaleY = 1.0f;
	float alpha = 1.0f, degree = 0.0f;

	if (style == iPopupStyleAlpha)
	{
		p = closePoint;
		if (state == iPopupStateOpen)
		{
			alpha = currAnimDt / animDt;
			currAnimDt += dt;
			if (currAnimDt > animDt)
			{
				state = iPopupStateProc;
				if (methodOpen)
					methodOpen(this);
			}
		}
		else if (state == iPopupStateProc)
		{
			alpha = 1.0f;
		}
		else
		{
			alpha = 1.0f - currAnimDt / animDt;
			currAnimDt += dt;
			if (currAnimDt > animDt)
			{
				isShow = false;
				if (methodClose)
					methodClose(this);
				return;
			}
		}
	}
	else if (style == iPopupStyleMove)
	{
		if (state == iPopupStateOpen)
		{
			p = linear(openPoint, closePoint, currAnimDt / animDt);
			currAnimDt += dt;
			if (currAnimDt > animDt)
			{
				state = iPopupStateProc;
				if (methodOpen)
					methodOpen(this);
			}
		}
		else if (state == iPopupStateProc)
		{
			p = closePoint;
		}
		else
		{
			p = linear(closePoint, openPoint, currAnimDt / animDt);
			currAnimDt += dt;
			if (currAnimDt > animDt)
			{
				isShow = false;
				if (methodClose)
					methodClose(this);
				return;
			}
		}
	}
	else if (style == iPopupStyleAlphaMove)
	{
		if (state == iPopupStateOpen)
		{
			alpha = currAnimDt / animDt;
			p = linear(openPoint, closePoint, alpha);
			currAnimDt += dt;
			if (currAnimDt > animDt)
			{
				state = iPopupStateProc;
				if (methodOpen)
					methodOpen(this);
			}
		}
		else if (state == iPopupStateProc)
		{
			alpha = 1.0f;
			p = closePoint;
		}
		else
		{
			alpha = 1.0f - currAnimDt / animDt;
			p = linear(openPoint, closePoint, alpha);
			currAnimDt += dt;
			if (currAnimDt > animDt)
			{
				isShow = false;
				if (methodClose)
					methodClose(this);
				return;
			}
		}
	}
	else if (style == iPopupStyleZoom)
	{
		if (state == iPopupStateOpen)
		{
			scaleX = scaleY = currAnimDt / animDt;
			p = linear(openPoint, closePoint, scaleX);
			currAnimDt += dt;
			if (currAnimDt > animDt)
			{
				state = iPopupStateProc;
				if (methodOpen)
					methodOpen(this);
			}
		}
		else if (state == iPopupStateProc)
		{
			scaleX = scaleY = 1.0f;
			p = closePoint;
		}
		else
		{
			scaleX = scaleY = 1.0f - currAnimDt / animDt;
			p = linear(openPoint, closePoint, scaleX);
			currAnimDt += dt;
			if (currAnimDt > animDt)
			{
				isShow = false;
				if (methodClose)
					methodClose(this);
				return;
			}
		}
	}
	else if (style == iPopupStyleRotate)
	{
	if (state == iPopupStateOpen)
	{
		scaleX = scaleY = currAnimDt / animDt;
		p = linear(openPoint, closePoint, scaleX);
		degree = scaleX * 360.0f;
		currAnimDt += dt;
		if (currAnimDt > animDt)
		{
			state = iPopupStateProc;
			if (methodOpen)
				methodOpen(this);
		}
	}
	else if (state == iPopupStateProc)
	{
		scaleX = scaleY = 1.0f;
		p = closePoint;
		degree = 0;
	}
	else
	{
		scaleX = scaleY = 1.0f - currAnimDt / animDt;
		p = linear(openPoint, closePoint, scaleX);
		degree = scaleX * 360.0f;
		currAnimDt += dt;
		if (currAnimDt > animDt)
		{
			isShow = false;
			if (methodClose)
				methodClose(this);
			return;
		}
	}
	}

	if (methodDrawBefore)
		methodDrawBefore(dt, this);

	fbo->bind(texPop);
	iFBO::clear(0, 0, 0, 0);
	setRGBA(1, 1, 1, 1);
	for (int i = 0; i < imgs->count; i++)
	{
		iImage* s = (iImage*)imgs->getObjectByIndex(i);
		s->paint(dt, iPointZero);
	}
	fbo->unbind();

	setRGBA(1, 1, 1, alpha);
	if (degree == 0.0f)
	{
		drawTexture(texPop, p.x, p.y, scaleX, scaleY, TOP | LEFT,
			0, 0, texPop->width, texPop->height, 2, degree, REVERSE_VER);
	}
	
	setRGBA(1, 1, 1, 1);

	if (methodDrawAfter)
		methodDrawAfter(dt, this);
}

void iPopup::addImage(iImage* img)
{
	imgs->addObject(img);
}

void iPopup::show(bool show)
{
	if (prevShow == show)
		return;

	prevShow = show;
	if (show)
	{
		isShow = true;
		state = iPopupStateOpen;
	}
	else
	{
		state = iPopupStateClose;
	}
	currAnimDt = 0.0f;
}
