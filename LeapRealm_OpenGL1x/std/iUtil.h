#pragma once

void saveFile(const char* fileName, const char* buffer, int len);
char* loadFile(const char* fileName, int& len);

#define sinDeg(degree) sinf(M_PI / 180 * (degree))
#define cosDeg(degree) cosf(M_PI / 180 * (degree))
