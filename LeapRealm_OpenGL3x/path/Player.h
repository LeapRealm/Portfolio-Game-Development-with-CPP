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

	iPoint getCurrMapPos();

	void paint(float dt) override;
	void move(float dt);
};
