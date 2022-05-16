#pragma once

#include "iOpenGL.h"
#include "iCommon.h"

#define BN -1
#define BA  0
#define BB  1
#define BC  2
#define BD  3
#define BI  4

struct iSTInfo
{
	const char*		fileNameFrag;

	const char*		fileNameTex[4];
	int				buf[4];

	TextureFilter	filter[4];
	TextureWrap		wrap[4];
};

struct iSize3D
{
	float width, height, dummy;
};

class iShadertoy
{
public:
	iShadertoy();
	virtual ~iShadertoy();

	void init();
	void load(iSTInfo* info);
	void free();

	void paint(float dt);
	void key(iKeyState state, iPoint p);

public:
	GLuint programID[5];

	Texture* texFbo[4][2];
	bool toggle[4];

	Texture* texs[5][4];
	int bufs[5][4];
	TextureFilter filter[5][4];
	TextureWrap wrap[5][4];

	float iTime;
	int iFrame;
	float iMouse[4];
	float iChannelTime[4];
	iSize3D iChannelResolution[4];
};

struct PathTexture
{
	char* path;
	Texture* tex;
};

void cleanAllTexture();
Texture* getPathTexture(const char* szFormat, ...);
