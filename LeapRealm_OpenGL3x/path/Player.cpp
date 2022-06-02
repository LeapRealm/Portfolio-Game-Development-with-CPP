#include "Player.h"

#include "iLogUI.h"
#include "Path.h"

Player::Player()
{
	currLocalPos = iPointZero;
	targetLocalPos = iPointZero;

	memset(path, 0x00, sizeof(iPoint) * 100);
	pathNum = 0;
	pathIdx = 0;

	speed = 0.0f;
	isDest = true;
}

iPoint Player::getCurrMapPos()
{
	return currLocalPos + mapRect.origin;
}

void Player::paint(float dt)
{
	setRGBA(1, 1, 1, 1);
	fillRect(getCurrMapPos().x - (TileWidth / 2),
			 getCurrMapPos().y - TileHeight,
			 TileWidth, TileHeight, TileWidth / 2.0f);
	setRGBA(1, 1, 1, 1);

	setRGBA(1, 0, 0, 1);
	fillRect(getCurrMapPos().x - 2.5f, getCurrMapPos().y - 2.5f, 5, 5);
	setRGBA(1, 1, 1, 1);

	if (currLocalPos != targetLocalPos)
	{
		move(dt);
	}
	else
	{
		pathIdx++;
		if (pathIdx < pathNum)
		{
			targetLocalPos = path[pathIdx];
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
	iPoint v = targetLocalPos - currLocalPos;
	float len = iPointLength(v);
	if (len == 0)
		return;
	v /= len;

	v *= speed * dt;

	if (currLocalPos.x < targetLocalPos.x)
	{
		currLocalPos.x += v.x;
		if (currLocalPos.x > targetLocalPos.x)
			currLocalPos.x = targetLocalPos.x;
	}
	else if (currLocalPos.x > targetLocalPos.x)
	{
		currLocalPos.x += v.x;
		if (currLocalPos.x < targetLocalPos.x)
			currLocalPos.x = targetLocalPos.x;
	}

	if (currLocalPos.y < targetLocalPos.y)
	{
		currLocalPos.y += v.y;
		if (currLocalPos.y > targetLocalPos.y)
			currLocalPos.y = targetLocalPos.y;
	}
	else if (currLocalPos.y > targetLocalPos.y)
	{
		currLocalPos.y += v.y;
		if (currLocalPos.y < targetLocalPos.y)
			currLocalPos.y = targetLocalPos.y;
	}
}
