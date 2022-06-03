#pragma once

#include "PathObject.h"
#include "iStd.h"

struct Tree : PathObject
{
	Tree();
	virtual ~Tree();

	iRect rect;

	void setPosByIndex(int x, int y);

	void paint(float dt) override;
};
