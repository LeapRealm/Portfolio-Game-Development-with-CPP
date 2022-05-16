#pragma once

#include "iCommon.h"

typedef Texture* (*MethodST)(const char* str);

class iStrTex
{
public:
	iStrTex(MethodST m = nullptr);
	virtual ~iStrTex();

	iStrTex& operator=(const char* s);

	void setString(const char* szFormat, ...);
	void drawString(float x, float y, int anchor, const char* szFormat, ...);

public:
	char* str;
	int strLength;
	Texture* tex;
	MethodST method;
};
