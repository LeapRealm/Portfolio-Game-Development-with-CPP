#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <ctime>
#include <cmath>

#include "iCommon.h"
#include "iPoint.h"
#include "iSize.h"
#include "iRect.h"
#include "iString.h"
#include "iMatrix.h"
#include "iLinkedList.h"
#include "iImage.h" 
#include "iPopup.h"
#include "iGraphics.h"
#include "iColor.h"
#include "iStrTex.h"

#include "iUtil.h"
#include "iSort.h"

#include "iParticleSystem.h"
#include "iShortestPath.h"
#include "iShadertoy.h"

#include "iOpenAL.h"
#include "iSound.h"
#include "iOpenGL.h"

#define va_start_end(szFormat, strText) {					\
	va_list args;											\
	va_start(args, szFormat);								\
	vsnprintf(strText, sizeof(strText), szFormat, args);	\
	va_end(args);											\
}

#define KEY_NONE	0
#define KEY_LEFT	1
#define KEY_RIGHT	2
#define KEY_UP		4
#define KEY_DOWN	8
#define KEY_SPACE	16
#define KEY_ENTER	32
#define KEY_DELETE	64
#define KEY_SHIFT	128

void setKeyPressed(bool isPressed, int key);
void updateKeyDown();

bool getKey(int key);
bool getKeyDown(int key);

#define DEV_WIDTH  1280
#define DEV_HEIGHT 720
extern iSize devSize;
extern iRect viewport;

void setZoom(iPoint center, float scale, float zoomDt, float delayDt);

void setRGBA(float r, float g, float b, float a);
void setRGBA(iColor4f color);
void getRGBA(float& r, float& g, float& b, float& a);

void clearScreen();

void setClip(int x, int y, int w, int h);

void setDotSize(float size);
float getDotSize();
void drawDot(float x, float y);
void drawDot(iPoint p);

void setLineWidth(float width);
float getLineWidth();
void drawLine(iPoint sp, iPoint ep);
void drawLine(float x0, float y0, float x1, float y1);

void drawRect(iRect rt, float radius = 0.0f, float degree = 0.0f);
void drawRect(float x, float y, float width, float height, float radius = 0.0f, float degree = 0.0f);
void fillRect(iRect rt, float radius = 0.0f, float degree = 0.0f);
void fillRect(float x, float y, float width, float height, float radius = 0.0f, float degree = 0.0f);

struct Texture
{
	uint32 texID;
	float width, height;
	uint32 potWidth, potHeight;
	int retainCount;
};

uint32 nextPOT(uint32 x);
Texture* createTextureWithRGBA(int width, int height, uint8* rgba);
Texture* createTexture(const char* szFormat, ...);
Texture** createTextureDivide(int numX, int numY, const char* szFormat, ...);
void freeTexture(Texture* tex);
void drawTexture(Texture* tex, iPoint p, int anchor = TOP | LEFT);
void drawTexture(Texture* tex, float x, float y, int anchor = TOP | LEFT);
void drawTexture(Texture* tex, float x, float y, float scaleX, float scaleY, int anchor = TOP | LEFT);
void drawTexture(Texture* tex, float x, float y, float scaleX, float scaleY, int anchor,
				 int imageX, int imageY, int imageWidth, int imageHeight,
				 int rotateXYZ, int degree, int reverse = REVERSE_NONE);

void setStringName(const char* sn);
const char* getStringName();
void setStringSize(float size);
float getStringSize();
void setStringBorder(float width);
float getStringBorder();
void setStringRGBA(float r, float g, float b, float a);
void getStringRGBA(float& r, float& g, float& b, float& a);
void setStringBorderRGBA(float r, float g, float b, float a);
void getStringBorderRGBA(float& r, float& g, float& b, float& a);
iRect rectOfString(const char* szFormat, ...);

Texture* createTexture(int width, int height);
