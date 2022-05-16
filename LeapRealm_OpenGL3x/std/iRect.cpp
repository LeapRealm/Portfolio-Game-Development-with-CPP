#include "iRect.h"

iRect::iRect()
{
	origin.x = 0;
	origin.y = 0;
	size.width = 0;
	size.height = 0;
}

iRect::iRect(const iRect& r)
{
	origin.x = r.origin.x;
	origin.y = r.origin.y;
	size.width = r.size.width;
	size.height = r.size.height;
}

iRect::iRect(float x, float y, float width, float height)
{
	origin.x = x;
	origin.y = y;
	size.width = width;
	size.height = height;
}
 
iRect& iRect::operator=(const iRect& r)
{
	origin.x = r.origin.x;
	origin.y = r.origin.y;
	size.width = r.size.width;
	size.height = r.size.height;

	return *this;
}

iRect& iRect::operator=(const iPoint& p)
{
	origin.x = p.x;
	origin.y = p.y;

	return *this;
}

iRect& iRect::operator=(const iSize& s)
{
	size.width = s.width;
	size.height = s.height;

	return *this;
}

iRect iRect::operator-(const iPoint& p)
{
	iRect rt;
	rt.origin = origin - p;
	rt.size = size;

	return rt;
}

iRect& iRect::operator+=(const iPoint& p)
{
	origin.x += p.x;
	origin.y += p.y;

	return *this;
}

iRect& iRect::operator+=(const iSize& s)
{
	size.width += s.width;
	size.height += s.height;

	return *this;
}

iRect iRectMake(float x, float y, float width, float height)
{
	iRect r;
	r.origin.x = x;
	r.origin.y = y;
	r.size.width = width;
	r.size.height = height;

	return r;
}

bool containPoint(iPoint p, iRect rt)
{
	return !(p.x < rt.origin.x                     || 
			 p.x > rt.origin.x + rt.size.width     || 
			 p.y < rt.origin.y                     ||
			 p.y > rt.origin.y + rt.size.height);
}

bool containRect(iRect src, iRect dst)
{
	return !(src.origin.x + src.size.width < dst.origin.x   ||
			 src.origin.x > dst.origin.x + dst.size.width   ||
			 src.origin.y + src.size.height < dst.origin.y  ||
			 src.origin.y > dst.origin.y + dst.size.height);
}
