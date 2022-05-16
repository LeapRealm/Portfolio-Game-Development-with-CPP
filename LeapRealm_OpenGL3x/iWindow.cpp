#include "iWindow.h"

#include "iStd.h"

wchar_t* utf8_to_utf16(const char* szFormat, ...)
{
	char szText[512];
	va_start_end(szFormat, szText)

	int length = MultiByteToWideChar(CP_UTF8, 0, szText, -1, nullptr, 0);
	wchar_t* ws = new wchar_t[length];
	MultiByteToWideChar(CP_UTF8, 0, szText, strlen(szText) + 1, ws, length);

	return ws;
}

char* utf16_to_utf8(wchar_t* str)
{
	int length = WideCharToMultiByte(CP_UTF8, 0, str, lstrlen(str), nullptr, 0, nullptr, nullptr);
	char* s = new char[length + 1];
	WideCharToMultiByte(CP_UTF8, 0, str, lstrlen(str), s, length, nullptr, nullptr);
	s[length] = 0;

	return s;
}
 