#include "Player.h"

#include "iLogUI.h"
#include "Path.h"

Player::Player()
{
	currPoint = iPointZero;
	targetPoint = iPointZero;

	memset(path, 0x00, sizeof(iPoint) * 100);
	pathNum = 0;
	pathIdx = 0;

	speed = 0.0f;
	isDest = true;
}

void Player::paint(float dt)
{
	setLineWidth(12);
	for (int i = 1; i < pathNum - 1; i++)
		drawLine(path[i] + mapRect.origin, path[i + 1] + mapRect.origin);
	setLineWidth(1);

	setRGBA(1, 1, 1, 1);
	fillRect(currPoint.x + mapRect.origin.x - (TileWidth / 2), currPoint.y + mapRect.origin.y - (TileHeight / 2), TileWidth, TileHeight, TileWidth / 2.0f);
	setRGBA(1, 1, 1, 1);

	if (currPoint != targetPoint)
	{
		move(dt);
	}
	else
	{
		pathIdx++;
		if (pathIdx < pathNum)
		{
			targetPoint = path[pathIdx];
		}
		else
		{
			if (isDest == false)
			{
				addLogMessage(MsgAttrWarning, "목적지에 도착했습니다.");
				isDest = true;
			}
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
