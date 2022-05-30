#pragma once

#include "iStd.h"

struct Player
{
	iPoint currPoint;
	iPoint targetPoint;

	iPoint path[100];
	int pathNum;
	int pathIdx;

	float speed;

	void paint(float dt);
	void move(float dt);
};
