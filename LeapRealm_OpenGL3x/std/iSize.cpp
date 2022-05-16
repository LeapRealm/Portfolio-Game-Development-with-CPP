#include "iSize.h"

iSize::iSize()
{
	width = 0;
	height = 0;
}

iSize::iSize(const iSize& s)
{
	width = s.width;
	height = s.height;
}

iSize::iSize(float width, float height)
{
	this->width = width;
	this->height = height;
}

bool iSize::operator==(const iSize& s)
{
	return width == s.width && height == s.height;
}

bool iSize::operator!=(const iSize& s)
{
	return width != s.width || height != s.height;
}
 
iSize& iSize::operator=(const iSize& s)
{
	width = s.width;
	height = s.height;

	return *this;
}

iSize iSize::operator+(const iSize& s)
{
	iSize r;
	r.width = width + s.width;
	r.height = height + s.height;

	return r;
}

iSize& iSize::operator+=(const iSize& s)
{
	width += s.width;
	height += s.height;

	return *this;
}

iSize iSize::operator-(const iSize& s)
{
	iSize r;
	r.width = width - s.width;
	r.height = height - s.height;

	return r;
}

iSize& iSize::operator-=(const iSize& s)
{
	width -= s.width;
	height -= s.height;

	return *this;
}

iSize iSize::operator*(float s)
{
	iSize sz;
	sz.width = width * s;
	sz.height = height * s;

	return sz;
}

iSize& iSize::operator*=(float s)
{
	width *= s;
	height *= s;

	return *this;
}

iSize iSize::operator/(float s)
{
	iSize sz;
	sz.width = width / s;
	sz.height = height / s;

	return sz;
}

iSize& iSize::operator/=(float s)
{
	width /= s;
	height /= s;

	return *this;
}

iSize iSizeMake(float width, float height)
{
	iSize r;
	r.width = width;
	r.height = height;

	return r;
}
