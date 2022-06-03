#pragma once

#include <Windows.h>
#include "Resource.h"

#ifdef _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#endif

#define GDIPVER 0x0110
#include <gdiplus.h>
#pragma comment (lib, "Gdiplus.lib")

using namespace Gdiplus;

wchar_t* utf8_to_utf16(const char* szFormat, ...);
char* utf16_to_utf8(const wchar_t* str);

#include <CommCtrl.h>
#pragma comment(lib, "comctl32")	// WC_BUTTON...
#pragma comment(lib, "msimg32")		// GradientFill...

#include "iCommon.h"

enum WndStyle
{
	WndStyleStatic = 0,
	WndStyleButton,
	WndStyleCheckBox,
	WndStyleRadio,
	WndStyleComboBox,
	WndStyleListBox,
	WndStyleEditBox,
	WndStyleOpenGL,
};

typedef void (*MethodCtrlUpdate)(HWND hwnd);

void loadCtrl();
void freeCtrl();
void addCtrl(HWND hwnd, WndStyle style, MethodCtrlUpdate method);
void updateCtrl(WPARAM wParam, LPARAM lParam);

typedef void (*MethodDragFile)(const char* path);
void acceptDrag(HWND hWnd, MethodDragFile method);
void updateDrag(WPARAM wParam, LPARAM lParam);

const char* openFileDialog(bool isOpen, LPCWSTR wFilter);
const char* openFileDialog(bool isOpen, const char* filter);

typedef void (*MethodChooseColor)(int r, int g, int b);
void showChooseColor(MethodChooseColor method);

HWND createWndStatic(int x, int y, int width, int height, const char* str);
HWND createWndButton(int x, int y, int width, int height, const char* str, MethodCtrlUpdate method);

HWND createWndCheckBox(int x, int y, int width, int height, const char* str, MethodCtrlUpdate method);
bool getWndCheckBox(HWND hwnd);
void setWndCheckBox(HWND hwnd, bool isChecked);

HWND createWndRadio(int x, int y, int width, int height, const char* str, MethodCtrlUpdate method);
bool getWndRadio(HWND hwnd);
void setWndRadio(HWND hwnd, bool isChecked);

HWND createWndComboBox(int x, int y, int width, int height, const char** str, int strNum, MethodCtrlUpdate method);
void addWndComboBox(HWND hwnd, int index, const char* str);
void addWndComboBox(HWND hwnd, const char** str, int strNum);
void removeWndComboBox(HWND hwnd, int index);
int indexWndComboBox(HWND hwnd);
int CountWndComboBox(HWND hwnd);
char* getWndComboBox(HWND hwnd, int index);
void setWndComboBox(HWND hwnd, int index);

HWND createWndListBox(int x, int y, int width, int height, const char** str, int strNum, MethodCtrlUpdate method);
void addWndListBox(HWND hwnd, int index, const char* str);
void addWndListBox(HWND hwnd, const char** str, int strNum);
void removeWndListBox(HWND hwnd, int index);
int indexWndListBox(HWND hwnd);
int CountWndListBox(HWND hwnd);
char* getWndListBox(HWND hwnd, int index);
void setWndListBox(HWND hwnd, int index);

HWND createWndGroup(int x, int y, int width, int height, const char* str);

enum WndEditStyle
{
	WndEditStyleAll = 0,
	WndEditStyleInt,
	WndEditStyleFloat,
};
HWND createWndEditBox(int x, int y, int width, int height, WndEditStyle style, const char* str, int strLenMax, MethodCtrlUpdate method);
HWND createWndEditBoxMultiline(int x, int y, int width, int height, const char* str, int strLenMax, MethodCtrlUpdate method);

void enableWnd(HWND hwnd, bool isEnable);
void setWndText(HWND hwnd, const char* szFormat, ...);
char* getWndText(HWND hwnd);
int getWndInt(HWND hwnd);
float getWndFloat(HWND hwnd);

typedef void (*MethodUpdateGL)(float dt);
typedef void (*MethodKeyGL)(iKeyState state, iPoint p);
HWND createWndOpenGL(int x, int y, int width, int height, MethodUpdateGL m0, MethodKeyGL m1);
