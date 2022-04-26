#pragma once

#include "iStd.h"

/*
	프로젝트 생성시 설정할것.
	1. 추가 디렉토리
	2. _CRT_SECURE_NO_WARNINGS
	_USE_MATH_DEFINES
	GDIPVER=0x0110
	GLEW_STATIC

	glew.c 프로젝트 포함
	각종 헤더파일을 아래경로에 복붙
	C : \Program Files(x86)\Windows Kits\10\Include\10.0.19041.0\um\gl
 */

void loadOpenGL(void* hdc);
void freeOpenGL();
typedef void (*MethodDraw)(float dt);
void drawOpenGL(float dt, MethodDraw m);
void resizeOpenGL(int width, int height);

void loadCursor();
void freeCursor();
void setCursor(iPoint position);
bool updateCursor(bool inClient);
void drawCursor(float dt);
