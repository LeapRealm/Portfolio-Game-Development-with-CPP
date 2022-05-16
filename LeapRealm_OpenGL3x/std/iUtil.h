#pragma once

void saveFile(const char* fileName, const char* buffer, int len);
char* loadFile(const char* fileName, int& len);
bool existFile(const char* fileName);
void removeFile(const char* fileName);

#define sinDeg(degree) sinf(M_PI / 180 * (degree))
#define cosDeg(degree) cosf(M_PI / 180 * (degree))
