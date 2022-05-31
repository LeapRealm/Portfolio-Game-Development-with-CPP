#include "Path.h"

#include "iLogUI.h"
#include "iStd.h"
#include "Player.h"

// TODO: 맵 교체 버튼

iRect mapRect;
int tileCountX, tileCountY, tileCountXY;

uint8 _mapData[3][16*16] = {
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
static uint8* mapData = _mapData[mapIdx];

iShortestPath* sp;
Player* player;

void loadPath()
{
	mapRect = iRectMake(devSize.width / 2 - 30, devSize.height / 2 - 210, 480, 480);
	tileCountX = (int)mapRect.size.width / TileWidth;
	tileCountY = (int)mapRect.size.height / TileHeight;
	tileCountXY = tileCountX * tileCountY;

	sp = new iShortestPath();
	sp->set(mapData, tileCountX, tileCountY, TileWidth, TileHeight);

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

		sp->set(mapData, tileCountX, tileCountY, TileWidth, TileHeight);

		player->currPoint = player->targetPoint = iPointMake(TileWidth / 2, TileHeight / 2);
		player->pathIdx = 0;
		player->pathNum = 0;
		player->isDest = true;

		addLogMessage(MsgAttrGeneral, "맵을 변경하였습니다.");
	}

	for (int j = 0; j < tileCountY; j++)
	{
		for (int i = 0; i < tileCountX; i++)
		{
			if (mapData[i + j * tileCountX] == I)
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
		if (ix < 0 || iy < 0 || ix > tileCountX - 1 || iy > tileCountY - 1)
			break;

		int idx = iy * tileCountX + ix;
		if (mapData[idx] == I)
			break;

		addLogMessage(MsgAttrNotice, "(%d,%d)로 이동합니다.", ix, iy);

		sp->run(player->currPoint, p - mapRect.origin, player->path, player->pathNum);
		player->pathIdx = 0;

		break;
	}
	}
	return false;
}
