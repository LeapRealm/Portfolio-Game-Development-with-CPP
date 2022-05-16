#pragma once

#include <gl/glew.h>
#include <gl/GL.h>
//#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/wglew.h>
#pragma comment(lib, "opengl32.lib")

#include "iCommon.h"

struct Vertex
{
	float position[4];
	float color[4];
	float uv[2];
};

class iFBO;
extern iFBO* fbo;
extern GLuint vao, vbo, vbe;
extern iMatrix* matrixProject;
extern iMatrix* matrixModelview;

void loadOpenGL(void* hdc);
void freeOpenGL();
typedef void (*MethodDraw)(float dt);
void drawOpenGL(float dt, MethodDraw m);
void resizeOpenGL(int width, int height);

enum iBlendFunc
{
	iBlendFuncAlpha,				// 일반적
	iBlendFuncPreMultipliedAlpha,	// 팝업, 백버퍼
	iBlendFuncAdd,					// 파티클
};

void setGlBlendFunc(iBlendFunc bf);

GLuint createProgramID(const char* pathVert, const char* pathFrag);
void freeProgramID(GLuint id);

class iFBO
{
public:
	iFBO(int width, int height);
	virtual ~iFBO();

	static void clear(float r, float g, float b, float a);
	void bind();
	void bind(Texture* tex);
	void unbind();

public:
	uint32 fbo;

	uint32 depthBuffer;
	Texture* tex;

	Texture** listTex;
	int listNum;
	int prevFBO;
};

enum TextureFilter
{
	Linear,		// GL_LINEAR
	Nearest,	// GL_NEAREST
	Mipmap,		// 
};

enum TextureWrap
{
	Clamp,		// GL_CLAMP_TO_EDGE
	Repeat,		// GL_REPEAT
};

void setTexture(TextureFilter filter, TextureWrap wrap);
void setTexture(TextureFilter minFilter, TextureFilter magFilter, TextureWrap wrap);
void applyTexture();

void setTexture(uint32 texID, TextureFilter filter, TextureWrap wrap);
void setTexture(uint32 texID, TextureFilter minFilter, TextureFilter magFilter, TextureWrap wrap = Clamp);
