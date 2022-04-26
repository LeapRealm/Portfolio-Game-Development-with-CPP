#pragma once

#include <Windows.h>
#define GDIPVER 0x0110
#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")

#include "iSize.h"
#include "iColor.h"

typedef Gdiplus::Image igImage;

class iGraphics
{
public:
	iGraphics();
	virtual ~iGraphics();

	static ULONG_PTR startGdiplus();
	static void stopGdiplus(ULONG_PTR token);

	void init(iSize size);
	void init(int width, int height);
	void clean();
	void clear(iColor4f c);
	Texture* getTexture();

	static void initGraphics(Gdiplus::Graphics* g);

	void setClip(int x, int y, int w, int h);

	void drawLine(iPoint sp, iPoint ep);
	void drawLine(float x0, float y0, float x1, float y1);

	void drawRect(iRect rt, float radius = 0.0f);
	void drawRect(float x, float y, float width, float height, float radius = 0.0f);
	void fillRect(iRect rt, float radius = 0.0f);
	void fillRect(float x, float y, float width, float height, float radius = 0.0f);

	static uint8* bmp2rgba(Gdiplus::Bitmap* bmp, uint32& width, uint32& height);

	static igImage* createIgImage(const char* szFormat, ...);
	igImage** createIgImageDivide(int numX, int numY, const char* szFormat, ...);
	void freeIgImage(igImage* tex);
	void drawIgImage(igImage* tex, iPoint p, int anchor = TOP | LEFT);
	void drawIgImage(igImage* tex, float x, float y, int anchor = TOP | LEFT);
	void drawIgImage(igImage* tex, float x, float y, float scaleX, float scaleY, int anchor = TOP | LEFT);
	void drawIgImage(igImage* tex, float x, float y, float scaleX, float scaleY, int anchor,	// dest
		int imageX, int imageY, int imageWidth, int imageHeight,								// src
		int rotateXYZ, int degree, int reverse = REVERSE_NONE);

	void drawString(float x, float y, int anchor, const char* szFormat, ...);
	static iRect rectOfString(const char* szFormat, ...);

public:
	Gdiplus::Bitmap* bmp;
	Gdiplus::Graphics* g;
};

struct RectOfString
{
	Gdiplus::Bitmap* bmp;
	Gdiplus::Graphics* g;

	RectOfString();
	virtual ~RectOfString();

	static void checkFontFamily(Gdiplus::FontFamily* ff, int& fontStyle);
	void drawString(const char* str);

	iRect rectOfString(const char* str);
};
