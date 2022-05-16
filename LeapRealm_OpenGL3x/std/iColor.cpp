#include "iColor.h"

iColor4f iColor4fMake(float r, float g, float b, float a)
{
	iColor4f c;
	c.r = r;
	c.g = g;
	c.b = b;
	c.a = a;

	return c;
}

iColor4b iColor4bMake(uint8 r, uint8 g, uint8 b, uint8 a)
{
	iColor4b c;
	c.r = r;
	c.g = g;
	c.b = b;
	c.a = a;

	return c;
}

iColor4f iColor4fMakeByb(uint8 r, uint8 g, uint8 b, uint8 a)
{
	iColor4f c;
	c.r = r / 255.0f;
	c.g = g / 255.0f;
	c.b = b / 255.0f;
	c.a = a / 255.0f;

	return c;
}

iColor4b iColor4bMakeByf(float r, float g, float b, float a)
{
	iColor4b c;
	c.r = r * 255.0f;
	c.g = g * 255.0f;
	c.b = b * 255.0f;
	c.a = a * 255.0f;

	return c;
}
