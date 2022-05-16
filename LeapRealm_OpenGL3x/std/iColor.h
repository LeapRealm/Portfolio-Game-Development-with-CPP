#pragma once

#include "iCommon.h"

struct iColor4f
{
	float r, g, b, a;
};

struct iColor4b
{
	uint8 r, g, b, a;
};

iColor4f iColor4fMake(float r, float g, float b, float a);
iColor4b iColor4bMake(uint8 r, uint8 g, uint8 b, uint8 a);
iColor4f iColor4fMakeByb(uint8 r, uint8 g, uint8 b, uint8 a);\
iColor4b iColor4bMakeByf(float r, float g, float b, float a);
 