#include "Path.h"

#include "iLogUI.h"
#include "iStd.h"
#include "Player.h"

// TODO: 맵 교체 버튼

iRect mapRect;

static uint8 _mapData[3][TileCountXY] = {
	{
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,I,I,I,I,I,I,I,1,1,1,1,1,
		1,1,1,1,I,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,I,1,1,1,1,I,1,1,1,1,1,1,
		1,1,1,1,I,1,1,1,1,I,I,I,I,I,I,I,
		1,1,1,1,I,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,I,I,I,I,I,I,I,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,I,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,I,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,I,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,I,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,I,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	},
	{
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,I,I,I,I,I,I,I,I,I,I,I,I,I,I,1,
		1,I,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,I,1,I,I,I,I,I,I,I,I,I,I,I,I,I,
		1,I,1,I,1,1,1,1,1,1,1,1,1,1,1,1,
		1,I,1,I,1,1,1,1,1,1,1,1,1,1,1,1,
		1,I,1,I,1,1,1,1,1,I,I,I,I,I,I,I,
		1,I,1,I,1,1,1,1,1,1,1,1,1,1,1,1,
		1,I,1,I,1,1,1,1,1,1,1,1,1,1,1,1,
		1,I,1,I,1,1,1,1,1,1,1,1,1,1,1,1,
		1,I,1,I,I,I,I,I,I,I,I,I,1,1,1,1,
		1,I,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,I,1,1,1,1,I,I,I,I,I,I,I,I,1,1,
		1,I,1,1,1,1,I,1,1,1,1,1,1,1,1,1,
		1,I,1,1,1,1,I,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,I,1,1,1,1,1,1,1,1,1,
	},
	{
		1,1,1,1,I,I,I,I,I,I,I,1,1,1,1,1,
		1,1,1,1,I,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,I,1,1,1,1,I,1,1,1,1,1,1,
		1,1,1,1,I,1,1,1,1,I,I,I,I,I,I,I,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,I,1,I,1,1,1,1,1,I,I,I,I,I,I,I,
		1,I,1,I,1,1,1,1,1,1,1,1,1,1,1,1,
		1,I,1,I,1,1,1,1,1,1,1,1,1,1,1,1,
		1,I,1,I,1,1,1,1,1,1,1,1,1,1,1,1,
		1,I,1,I,I,I,I,I,I,I,I,I,1,1,1,1,
		1,I,1,I,1,1,1,1,1,1,1,1,1,1,1,1,
		1,1,1,I,1,1,1,1,1,1,1,1,1,1,1,1,
		1,I,1,I,1,1,1,1,1,I,I,I,I,I,I,I,
		1,I,1,I,1,1,1,1,1,1,1,1,1,1,1,1,
		1,I,1,I,1,1,1,I,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,I,I,1,1,1,1,1,1,1,1,
	},
};

static int mapIdx = 0;
uint8* mapData = _mapData[mapIdx];

iShortestPath* sp;
Player* player;

void loadPath()
{
	mapRect = iRectMake(devSize.width / 2 - TileWidth, 
						devSize.height / 2 - (TileCountX * TileWidth / 2 - TileHeight), 
						TileCountX * TileWidth, TileCountY * TileHeight);

	sp = new iShortestPath();
	sp->set(mapData, TileCountX, TileCountY, TileWidth, TileHeight);

	player = new Player;
	player->speed = 400;
	player->currPoint = player->targetPoint = iPointMake(TileWidth / 2, TileHeight / 2);
}

void freePath()
{
	delete player;
	delete sp;
}

void drawPath(float dt)
{
	if (getActionKeyPressed(KEY_SPACE))
	{
		mapIdx++;
		mapIdx %= 3;
		mapData = _mapData[mapIdx];

		sp->set(mapData, TileCountX, TileCountY, TileWidth, TileHeight);

		int ix = player->currPoint.x / TileWidth;
		int iy = player->currPoint.y / TileHeight;
		int idx = iy * TileCountX + ix;

		if (mapData[idx] == I)
		{
			addLogMessage(MsgAttrWarning, "벽이 존재하여 현재 위치가 초기화되었습니다.");
			player->currPoint = player->targetPoint = iPointMake(TileWidth / 2, TileHeight / 2);
		}
		else
		{
			player->targetPoint = player->currPoint;
		}

		player->pathIdx = 0;
		player->pathNum = 0;
		player->isDest = true;

		addLogMessage(MsgAttrGeneral, "맵을 변경하였습니다.");
	}

	for (int j = 0; j < TileCountY; j++)
	{
		for (int i = 0; i < TileCountX; i++)
		{
			if (mapData[i + j * TileCountX] == I)
			{
				setRGBA(1.0f, 0.5f, 0, 0.8f);
				fillRect((TileWidth * i + 2) + mapRect.origin.x, 
						 (TileHeight * j + 2) + mapRect.origin.y,
						  TileWidth - 4, TileHeight - 4);
			}
			else
			{
				setRGBA(1, 1, 1, 0.8f);
				drawRect((TileWidth * i + 2) + mapRect.origin.x, 
						 (TileHeight * j + 2) + mapRect.origin.y,
						  TileWidth - 4, TileHeight - 4);
			}
		}
	}
	setRGBA(1, 1, 1, 1);

	player->paint(dt);
}

bool keyPath(iKeyState state, iPoint p)
{
	switch (state)
	{
	case iKeyStateBegan:
	{
		int px = (int)p.x - mapRect.origin.x;
		int py = (int)p.y - mapRect.origin.y;
		if (px < 0 || py < 0)
			break;

		int ix = px / TileWidth;
		int iy = py / TileHeight;
		if (ix < 0 || iy < 0 || ix > TileCountX - 1 || iy > TileCountY - 1)
			break;

		int idx = iy * TileCountX + ix;
		if (mapData[idx] == I)
			break;

		player->isDest = false;
		addLogMessage(MsgAttrNotice, "(%d,%d)로 이동합니다.", ix, iy);

		sp->run(player->currPoint, p - mapRect.origin, player->path, player->pathNum);
		player->pathIdx = 0;

		break;
	}
	}
	return false;
}
