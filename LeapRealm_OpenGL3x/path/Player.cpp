#include "Player.h"

#include "Path.h"

void Player::paint(float dt)
{
	setLineWidth(5);
	for (int i = 1; i < pathNum - 1; i++)
		drawLine(path[i], path[i + 1]);
	setLineWidth(1);

	fillRect(currPoint.x, currPoint.y, TileWidth, TileHeight, TileWidth / 2.0f);

	if (currPoint != targetPoint)
	{
		move(dt);
	}
	else
	{
		pathIdx++;
		if (pathIdx < pathNum)
		{
			targetPoint = path[pathIdx] - iPointMake(TileWidth / 2.0f, TileHeight / 2.0);
		}
		else
		{
			pathIdx = 0;
			pathNum = 0;
		}
	}
}

void Player::move(float dt)
{
	iPoint v = targetPoint - currPoint;
	float len = iPointLength(v);
	if (len == 0)
		return;
	v /= len;

	v *= speed * dt;

	if (currPoint.x < targetPoint.x)
	{
		currPoint.x += v.x;
		if (currPoint.x > targetPoint.x)
			currPoint.x = targetPoint.x;
	}
	else if (currPoint.x > targetPoint.x)
	{
		currPoint.x += v.x;
		if (currPoint.x < targetPoint.x)
			currPoint.x = targetPoint.x;
	}

	if (currPoint.y < targetPoint.y)
	{
		currPoint.y += v.y;
		if (currPoint.y > targetPoint.y)
			currPoint.y = targetPoint.y;
	}
	else if (currPoint.y > targetPoint.y)
	{
		currPoint.y += v.y;
		if (currPoint.y < targetPoint.y)
			currPoint.y = targetPoint.y;
	}
}
