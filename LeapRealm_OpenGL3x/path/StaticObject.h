#pragma once

#include "PathObject.h"
#include "iStd.h"

struct StaticObject : PathObject
{
	StaticObject();
	virtual ~StaticObject();

	iRect rect;
	bool isOverlap;

	Texture* tex;

	void setPosByIndex(int x, int y);

	void paint(float dt) override;
};
