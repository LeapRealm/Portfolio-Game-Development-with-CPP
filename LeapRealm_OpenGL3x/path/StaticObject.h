#pragma once

#include "PathObject.h"
#include "iStd.h"

struct StaticObject : PathObject
{
	StaticObject();

	iRect rect;
	bool isOverlap;

	void setPosByIndex(int x, int y);

	void paint(float dt) override;
};
