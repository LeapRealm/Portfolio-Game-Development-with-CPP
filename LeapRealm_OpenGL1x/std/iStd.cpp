#include "iStd.h"

#include "iColor.h"
#include "iGraphics.h"
#include "iWindow.h"

static uint32 pressedAxisKeys = 0;
static uint32 pressedActionKeys = 0;
 
static void _setKeyPressed(uint32& keys, bool isPressed, int key)
{
	if (isPressed)
	{
		switch (key)
		{
			case 'a': case 'A':case 37: keys |= KEY_LEFT;	 break;
			case 'd': case 'D':case 39: keys |= KEY_RIGHT;	 break;
			case 'w': case 'W':case 38: keys |= KEY_UP;		 break;
			case 's': case 'S':case 40: keys |= KEY_DOWN;	 break;
			case 32:					keys |= KEY_SPACE;	 break;
			case 13:					keys |= KEY_ENTER;	 break;
			case 46:					keys |= KEY_DELETE;  break;
			case 16:					keys |= KEY_SHIFT;	 break;
		}
	}
	else
	{
		switch (key)
		{
			case 'a': case 'A':case 37: keys &= ~KEY_LEFT;	 break;
			case 'd': case 'D':case 39: keys &= ~KEY_RIGHT;  break;
			case 'w': case 'W':case 38: keys &= ~KEY_UP;	 break;
			case 's': case 'S':case 40: keys &= ~KEY_DOWN;	 break;
			case 32:					keys &= ~KEY_SPACE;  break;
			case 13:					keys &= ~KEY_ENTER;  break;
			case 46:					keys &= ~KEY_DELETE; break;
			case 16:					keys &= ~KEY_SHIFT;  break;
		}
	}
}

void setKeyPressed(bool isPressed, int key)
{
	if (isPressed)
	{
		_setKeyPressed(pressedAxisKeys, isPressed, key);
		_setKeyPressed(pressedActionKeys, isPressed, key);
	}
	else
	{
		_setKeyPressed(pressedAxisKeys, isPressed, key);
	}
}

void updateActionKeyPressed()
{
	pressedActionKeys = 0;
}

bool getAxisKeyPressed(int key)
{
	return pressedAxisKeys & key;
}

bool getActionKeyPressed(int key)
{
	return pressedActionKeys & key;
}

iSize devSize;
iRect viewport;

static float cr, cg, cb, ca;

void setRGBA(float r, float g, float b, float a)
{
	cr = r;
	cg = g;
	cb = b;
	ca = a;
}

void setRGBA(iColor4f color)
{
	cr = color.r;
	cg = color.g;
	cb = color.b;
	ca = color.a;
}

void getRGBA(float& r, float& g, float& b, float& a)
{
	r = cr;
	g = cg;
	b = cb;
	a = ca;
}

void clearScreen()
{
	iFBO::clear(cr, cg, cb, ca);
}

void setClip(int x, int y, int w, int h)
{
	if (x == 0 && y == 0 && w == 0 && h == 0)
	{
		glDisable(GL_SCISSOR_TEST);
	}
	else
	{
		// glViewport 실제 모니터에서의 사이즈
		glEnable(GL_SCISSOR_TEST);
		glScissor(x, devSize.height - y - h, w, h);
	}
}

void setDotSize(float size)
{
	glPointSize(size);
}

void drawDot(float x, float y)
{
	drawDot(iPointMake(x, y));
}

static TextureFilter _minFilter, _magFilter;
static TextureWrap _wrap;
void setTexture(TextureFilter filter, TextureWrap wrap)
{
	_minFilter = _magFilter = filter;
	_wrap = wrap;
}

void setTexture(TextureFilter minFilter, TextureFilter magFilter, TextureWrap wrap)
{
	_minFilter = minFilter;
	_magFilter = magFilter;
	_wrap = wrap;
}

void applyTexture()
{
	GLenum e = (_wrap == Clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, e);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, e);

	e = (_minFilter == Nearest ? GL_NEAREST : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, e);

	e = (_magFilter == Nearest ? GL_NEAREST : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, e);
}

void setTexture(uint32 texID, TextureFilter filter, TextureWrap wrap)
{
	setTexture(texID, filter, filter, wrap);
}

void setTexture(uint32 texID, TextureFilter minFilter, TextureFilter magFilter, TextureWrap wrap)
{
	glBindTexture(GL_TEXTURE_2D, texID);

	GLenum e = (wrap == Clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, e);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, e);

	e = (minFilter == Nearest ? GL_NEAREST : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, e);

	e = (magFilter == Nearest ? GL_NEAREST : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, e);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void drawDot(iPoint p)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(2, GL_FLOAT, 0, &p);
	iColor4f c = { cr, cg, cb, ca };
	glColorPointer(4, GL_FLOAT, 0, &c);

	glDrawArrays(GL_POINTS, 0, 1);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

float lineWidth = 1.0f;
void setLineWidth(float width)
{
	lineWidth = width;
	glLineWidth(width);
}

float getLineWidth()
{
	return lineWidth;
}

void drawLine(float x0, float y0, float x1, float y1)
{
	drawLine(iPointMake(x0, y0), iPointMake(x1, y1));
}

void drawLine(iPoint sp, iPoint ep)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	iPoint p[2] = { sp, ep };
	iColor4f c[2] = { { cr, cg, cb, ca },{ cr, cg, cb, ca } };
	glVertexPointer(2, GL_FLOAT, 0, p);
	glColorPointer(4, GL_FLOAT, 0, c);

	glDrawArrays(GL_LINES, 0, 2);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void drawPoly(iPoint* p, int pNum, bool fill)
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(2, GL_FLOAT, 0, p);
	glColor4f(cr, cg, cb, ca);

	if (fill)
		glDrawArrays(GL_TRIANGLE_FAN, 0, pNum);
	else
		glDrawArrays(GL_LINE_LOOP, 0, pNum);

	glDisableClientState(GL_VERTEX_ARRAY);
}

void drawRect(iRect rt, float radius)
{
	drawRect(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height, radius);
}

void drawRect(float x, float y, float width, float height, float radius)
{
	iPoint p[4];
	p[0] = iPointMake(x, y);
	p[1] = iPointMake(x + width, y);
	p[2] = iPointMake(x + width, y + height);
	p[3] = iPointMake(x, y + height);
	drawPoly(p, 4, false);
}

void fillRect(iRect rt, float radius)
{
	fillRect(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height, radius);
}

void fillRect(float x, float y, float width, float height, float radius)
{
	iPoint p[4];
	p[0] = iPointMake(x, y);
	p[1] = iPointMake(x + width, y);
	p[2] = iPointMake(x + width, y + height);
	p[3] = iPointMake(x, y + height);
	drawPoly(p, 4, true);
}

int texNum = 0;

uint32 nextPOT(uint32 x)
{
#if 0
	int n;
	for (n = 0; x > 0; n++)
		x /= 2;

	x = 1;
	for (int i = 0; i < n; i++)
		x *= 2;

	return x;
#else
	x = x - 1;
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	x = x | (x >> 8);
	x = x | (x >> 16);

	return x + 1;
#endif
}

Texture* createTextureWithRGBA(int width, int height, uint8* rgba)
{
	int potWidth = nextPOT(width);
	int potHeight = nextPOT(height);

	uint32 texID = 0;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	applyTexture();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, potWidth, potHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);

	glBindTexture(GL_TEXTURE_2D, 0);

	Texture* tex = new Texture;
	tex->texID = texID;
	tex->width = width;
	tex->height = height;
	tex->potWidth = potWidth;
	tex->potHeight = potHeight;
	tex->retainCount = 1;

	texNum++;

	return tex;
}

uint8* createImageData(uint32& width, uint32& height, const char* szFormat)
{
	wchar_t* ws = utf8_to_utf16(szFormat);
	Bitmap* bmp = Bitmap::FromFile(ws);
	delete ws;

	uint8* rgba = iGraphics::bmp2rgba(bmp, width, height);
	delete bmp;

	return rgba;
}

Texture* createTexture(const char* szFormat, ...)
{
	char szText[256];
	va_start_end(szFormat, szText)

	uint32 width, height;
	uint8* rgba = createImageData(width, height, szText);

	Texture* tex = createTextureWithRGBA(width, height, rgba);
	delete rgba;

	return tex;
}

Texture** createTextureDivide(int numX, int numY, const char* szFormat, ...)
{
	char szText[256];
	va_start_end(szFormat, szText)

	wchar_t* ws = utf8_to_utf16(szText);
	Bitmap* bmp = Bitmap::FromFile(ws);
	delete ws;

	uint32 width, height;
	uint8* rgba = iGraphics::bmp2rgba(bmp, width, height);
	delete bmp;

	Texture** texs = new Texture * [numX * numY];
	int w = width / numX;
	int h = height / numY;
	int pw = nextPOT(w);
	int ph = nextPOT(h);
	uint8* newRGBA = new uint8[pw * ph * 4];

	for (int j = 0; j < numY; j++)
	{
		for (int i = 0; i < numX; i++)
		{
			memset(newRGBA, 0x00, sizeof(uint8) * pw * ph * 4);
			for (int n = 0; n < h; n++)
			{
				for (int m = 0; m < w; m++)
				{
					uint8* dst = &newRGBA[pw * 4 * n + 4 * m];
					uint8* src = &rgba[width * 4 * (h * j + n) + 4 * (w * i + m)];
					memcpy(dst, src, sizeof(uint8) * 4);
				}
			}

			texs[numX * j + i] = createTextureWithRGBA(w, h, newRGBA);
		}
	}
	delete newRGBA;
	delete rgba;

	return texs;
}

void freeTexture(Texture* tex)
{
	if (tex->retainCount > 1)
	{
		tex->retainCount--;
		return;
	}
	texNum--;
	glDeleteTextures(1, &tex->texID);
	delete tex;
}

void drawTexture(Texture* tex, iPoint p, int anchor)
{
	drawTexture(tex, p.x, p.y, 1.0f, 1.0f, anchor, 0, 0, tex->width, tex->height, 2, 0);
}

void drawTexture(Texture* tex, float x, float y, int anchor)
{
	drawTexture(tex, x, y, 1.0f, 1.0f, anchor, 0, 0, tex->width, tex->height, 2, 0);
}

void drawTexture(Texture* tex, float x, float y, float scaleX, float scaleY, int anchor)
{
	drawTexture(tex, x, y, scaleX, scaleY, anchor, 0, 0, tex->width, tex->height, 2, 0);
}

void drawTexture(Texture* tex, float x, float y, float scaleX, float scaleY, int anchor,
	int imageX, int imageY, int imageWidth, int imageHeight,
	int rotateXYZ, int degree, int reverse)
{
	float dx = imageWidth * scaleX;
	float dy = imageHeight * scaleY;

	switch (anchor)
	{
	case TOP | LEFT:										break;
	case TOP | RIGHT:			x -= dx;					break;
	case TOP | HCENTER:			x -= dx / 2;				break;
	case BOTTOM | LEFT:						 y -= dy;		break;
	case BOTTOM | RIGHT:		x -= dx;	 y -= dy;		break;
	case BOTTOM | HCENTER:		x -= dx / 2; y -= dy;		break;
	case VCENTER | LEFT:					 y -= dy / 2;	break;
	case VCENTER | RIGHT:		x -= dx;	 y -= dy / 2;	break;
	case VCENTER | HCENTER:		x -= dx / 2; y -= dy / 2;	break;
	}

	iPoint p[4] =
	{
		{x, y},			{x + dx, y},
		{x, y + dy},	{x + dx, y + dy},
	};

	if (reverse & REVERSE_HOR)
	{
		float t = p[0].x;
		p[0].x = p[1].x;
		p[1].x = t;
		p[2].x = p[0].x;
		p[3].x = p[1].x;
	}
	if (reverse & REVERSE_VER)
	{
		float t = p[0].y;
		p[0].y = p[2].y;
		p[2].y = t;
		p[1].y = p[0].y;
		p[3].y = p[2].y;
	}

	iColor4f c[4] = {
		{cr, cg, cb, ca},					{cr, cg, cb, ca},
		{cr, cg, cb, ca},					{cr, cg, cb, ca},
	};

	iPoint st[4] = {
		{0, 0},								{tex->width/tex->potWidth, 0},
		{0, tex->height/tex->potHeight},	{tex->width/tex->potWidth, tex->height/tex->potHeight},
	};

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); // backup
	glLoadIdentity();

	if (degree)
	{
		p[0] = iPointMake(-dx / 2, -dy / 2);	p[1] = iPointMake(+dx / 2, -dy / 2);
		p[2] = iPointMake(-dx / 2, +dy / 2);	p[3] = iPointMake(+dx / 2, +dy / 2);

		glTranslatef(x + dx / 2, y + dy / 2, 0);
		glRotatef(degree, rotateXYZ == 0, rotateXYZ == 1, rotateXYZ == 2);
	}

	glVertexPointer(2, GL_FLOAT, 0, p);
	glColorPointer(4, GL_FLOAT, 0, c);
	glTexCoordPointer(2, GL_FLOAT, 0, st);
	glBindTexture(GL_TEXTURE_2D, tex->texID);

	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	uint8 indices[6] = { 0,1,2, 1,2,3 };
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

	glPopMatrix(); // restore glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
}

static char* stringName = nullptr;
static float stringSize, stringBorder;
static iColor4f stringColor, stringBorderColor;

void setStringName(const char* sn)
{
	if (stringName)
	{
		if (strcmp(stringName, sn) == 0)
			return;
		delete stringName;
	}
	int len = strlen(sn);
	stringName = new char[len + 1];
	strcpy(stringName, sn);
}

const char* getStringName()
{
	return stringName;
}

void setStringSize(float size)
{
	stringSize = size;
}

float getStringSize()
{
	return stringSize;
}

void setStringBorder(float width)
{
	stringBorder = width;
}

float getStringBorder()
{
	return stringBorder;
}

void setStringRGBA(float r, float g, float b, float a)
{
	stringColor = iColor4fMake(r, g, b, a);
}

void getStringRGBA(float& r, float& g, float& b, float& a)
{
	r = stringColor.r;
	g = stringColor.g;
	b = stringColor.b;
	a = stringColor.a;
}

void setStringBorderRGBA(float r, float g, float b, float a)
{
	stringBorderColor = iColor4fMake(r, g, b, a);
}

void getStringBorderRGBA(float& r, float& g, float& b, float& a)
{
	r = stringBorderColor.r;
	g = stringBorderColor.g;
	b = stringBorderColor.b;
	a = stringBorderColor.a;
}

iRect rectOfString(const char* szFormat, ...)
{
	char szText[512];
	va_start_end(szFormat, szText);
	return iGraphics::rectOfString(szText);
}

char* str = nullptr;
Texture* texString = nullptr;

void drawString(float x, float y, int anchor, const char* szFormat, ...)
{
#if 0
	#warning "Don't use this fuction."
	return;
#endif

	char szText[512];
	va_start_end(szFormat, szText);

	if (str == nullptr || strcmp(str, szText))
	{
		iRect rt = rectOfString(szText);

		iGraphics* g = new iGraphics();
		g->init(rt.size);

		setRGBA(0, 1, 0, 0.8);
		g->fillRect(0, 0, rt.size.width, rt.size.height);
		setRGBA(1, 1, 1, 1);

		g->drawString(0, 0, TOP | LEFT, szText);

		if (str)
			delete str;
		str = new char[strlen(szText) + 1];
		strcpy(str, szText);

		if (texString)
			freeTexture(texString);
		texString = g->getTexture();
		delete g;
	}

	drawTexture(texString, x, y, anchor);
}

Texture* createTexture(int width, int height)
{
	uint32 texID = 0;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	applyTexture();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glBindTexture(GL_TEXTURE_2D, 0);

	Texture* tex = new Texture;
	tex->texID = texID;
	tex->width = width;
	tex->height = height;
	tex->potWidth = width;
	tex->potHeight = height;
	tex->retainCount = 1;

	texNum++;

	return tex;
}

iFBO* fbo;

iFBO::iFBO(int width, int height)
{
	// render(depth) buffer
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// color buffer
	tex = createTexture(width, height);

	// fbo
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLenum fboBuffs[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, fboBuffs);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->texID, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	listTex = new Texture * [20];
	listNum = 0;
}

iFBO::~iFBO()
{
	glDeleteRenderbuffers(1, &depthBuffer);

	freeTexture(tex);

	glDeleteFramebuffers(1, &fbo);

	delete listTex;
}

void iFBO::clear(float r, float g, float b, float a)
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void iFBO::bind()
{
	bind(tex);
}

void iFBO::bind(Texture* tex)
{
	if (listNum == 0)
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFBO);
	listTex[listNum] = tex;
	listNum++;

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLenum fboBuffs[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, fboBuffs);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->texID, 0);

	glViewport(0, 0, tex->width, tex->height);
}

void iFBO::unbind()
{
	listNum--;
	if (listNum)
	{
		bind(listTex[listNum - 1]);
		listNum--;
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, prevFBO);
		glViewport(viewport.origin.x, viewport.origin.y, viewport.size.width, viewport.size.height);
	}
}

void setGlBlendFunc(iBlendFunc bf)
{
	switch (bf)
	{
	case iBlendFuncAlpha: // default
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		break;

	case iBlendFuncPreMultipliedAlpha:
		glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		break;

	case iBlendFuncAdd:
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		break;
	}
}
