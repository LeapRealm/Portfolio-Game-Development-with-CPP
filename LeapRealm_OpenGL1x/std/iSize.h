#pragma once

struct iSize
{
	float width, height;

	iSize();
	iSize(const iSize& s);
	iSize(float width, float height);

	bool operator==(const iSize& s);
	bool operator!=(const iSize& s);

	iSize& operator=(const iSize& s);

	iSize operator+(const iSize& s);
	iSize& operator+=(const iSize& s);

	iSize operator-(const iSize& s);
	iSize& operator-=(const iSize& s);

	iSize operator*(float s);
	iSize& operator*=(float s);

	iSize operator/(float s);
	iSize& operator/=(float s);
};

#define iSizeZero iSizeMake(0, 0)
iSize iSizeMake(float width, float height);
 