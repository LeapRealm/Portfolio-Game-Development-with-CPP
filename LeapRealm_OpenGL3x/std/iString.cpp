#include "iString.h"

#include "iStd.h"

iString::iString()
{
	str = new char[default_length];
	strLength = default_length;
}

iString::iString(const char* szFormat, ...)
{
	str = new char[default_length];
	strLength = default_length;

	char szText[512];
	va_start_end(szFormat, szText)
	copy(szText);
}
 
iString::iString(const iString& s)
{
	str = new char[default_length];
	strLength = default_length;

	copy(s.str);
}

iString::~iString()
{
	delete str;
}

iString& iString::operator=(const char* s)
{
	copy(s);
	return *this;
}

iString& iString::operator=(const iString& s)
{
	copy(s.str);
	return *this;
}

iString& iString::operator+=(const char* s)
{
	concat(s);
	return *this;
}

iString& iString::operator+=(const iString& s)
{
	concat(s.str);
	return *this;
}

static char* buffer = nullptr;
int bufferLength = 0;
const char* iString::operator+(const char* s)
{
	int len0 = strlen(str);
	int len1 = strlen(s);
	if (buffer == nullptr)
	{
		buffer = new char[len0 + len1 + 1];
		bufferLength = len0 + len1 + 1;
	}
	else
	{
		if (bufferLength < len0 + len1 + 1)
		{
			delete buffer;
			buffer = new char[len0 + len1 + 1];
			bufferLength = len0 + len1 + 1;
		}
	}

	char* r = buffer;
	strcpy(r, str);
	strcpy(&r[len0], s);

	return r;
}

const char* iString::operator+(const iString& _s)
{
	const char* s = _s.str;

	int len0 = strlen(str);
	int len1 = strlen(s);
	if (buffer == nullptr)
	{
		buffer = new char[len0 + len1 + 1];
		bufferLength = len0 + len1 + 1;
	}
	else
	{
		if (bufferLength < len0 + len1 + 1)
		{
			delete buffer;
			buffer = new char[len0 + len1 + 1];
			bufferLength = len0 + len1 + 1;
		}
	}

	char* r = buffer;
	strcpy(r, str);
	strcpy(&r[len0], s);

	return r;
}

void iString::copy(const char* s)
{
	int len = strlen(s);
	if (strLength < len + 1)
	{
		delete str;
		str = new char[len + 1];
		strLength = len + 1;
	}
	strcpy(str, s);
}

void iString::concat(const char* s)
{
	int len0 = strlen(str);
	int len1 = strlen(s);
	if (strLength < len0 + len1 + 1)
	{
		char* newStr = new char[len0 + len1 + 1];
		strcpy(newStr, str);
		delete str;
		str = newStr;
		strLength = len0 + len1 + 1;
	}
	strcpy(&str[len0], s);
}

char** iString::getStringLine(const char* str, int& lineNum, int d)
{
	lineNum = 1;
	for (int i = 0; str[i]; i++)
	{
		if (str[i] == d)
			lineNum++;
	}
	char** line = new char* [lineNum];
	int i, off = 0, n = 0;
	for (i = 0; str[i]; i++)
	{
		if (str[i] == d)
		{
			int len = i - off;
			char* s = new char[len + 1];
			memcpy(s, &str[off], len);
			s[len] = 0;
			line[n] = s; n++;
			off = i + 1;
		}
	}

	int len = strlen(str) - off;
	char* s = new char[len + 1];
	memcpy(s, &str[off], len);
	s[len] = 0;
	line[n] = s;
	return line;
}

void iString::freeStringLine(char** line, int lineNum)
{
	for (int i = 0; i < lineNum; i++)
		delete line[i];
	delete line;
}
