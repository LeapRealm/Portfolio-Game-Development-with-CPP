#include "iUtil.h"

#include "iStd.h"

void saveFile(const char* fileName, const char* buffer, const int len)
{
	FILE* fp = fopen(fileName, "wb");

	fwrite(buffer, sizeof(char), len, fp);

	fclose(fp);
}

char* loadFile(const char* fileName, int& len)
{
	FILE* fp = fopen(fileName, "rb");

	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char* buffer = new char[len + 1];
	fread(buffer, sizeof(char), len, fp);
	buffer[len] = 0;
 
	fclose(fp);

	return buffer;
}
