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
