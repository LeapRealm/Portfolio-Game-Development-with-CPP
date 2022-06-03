#pragma once

#include "iStd.h"
#include "PathObject.h"

struct Player : PathObject
{
	Player();

	iPoint currLocalPos;
	iPoint targetLocalPos;

	iPoint path[100];
	int pathNum;
	int pathIdx;

	float speed;
	bool isDest;

	iPoint getCurrWorldPos();

	void paint(float dt) override;
	void move(float dt);
};
