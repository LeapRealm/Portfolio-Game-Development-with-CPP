#pragma once

struct iPoint
{
	float x, y;

	iPoint();
	iPoint(const iPoint& p);
	iPoint(float x, float y);

	bool operator==(const iPoint& p);
	bool operator!=(const iPoint& p);

	iPoint& operator=(const iPoint& p);

	iPoint operator+(const iPoint& p);
	iPoint& operator+=(const iPoint& p);

	iPoint operator-(const iPoint& p);
	iPoint& operator-=(const iPoint& p);

	iPoint operator*(float s);
	iPoint& operator*=(float s);

	iPoint operator/(float s);
	iPoint& operator/=(float s);

	float length();
};
 
#define iPointZero iPointMake(0, 0)
iPoint iPointMake(float x, float y);
float iPointLength(iPoint p);
float iPointDistance(iPoint p0, iPoint p1);
iPoint iPointRotate(iPoint p, iPoint pivot, float degree);
float iPointAngle(const iPoint& v1, const iPoint& v2, const iPoint& v3);
float setDegree(float& degree, iPoint att, iPoint def, float spin, float dt);

float linear(float s, float e, float r);
iPoint linear(iPoint s, iPoint e, float r);

float easeIn(float s, float e, float r);
iPoint easeIn(iPoint s, iPoint e, float r);

float easeOut(float s, float e, float r);
iPoint easeOut(iPoint s, iPoint e, float r);

float easeInOut(float s, float e, float r);
iPoint easeInOut(iPoint s, iPoint e, float r);

bool movePoint(iPoint& p, iPoint sp, iPoint ep, float speedDt);
iPoint normalize(iPoint p);
