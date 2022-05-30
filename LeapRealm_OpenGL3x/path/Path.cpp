#include "Path.h"

#include "iStd.h"
#include "Player.h"

int tileCountX, tileCountY, tileCountXY;

uint8* mapData;
iShortestPath* sp;
Player* player;

void loadPath()
{
	tileCountX = (int)(devSize.width) / TileWidth;
	tileCountY = (int)(devSize.height) / TileHeight;
	tileCountXY = tileCountX * tileCountY;

	mapData = new uint8[tileCountXY];
	for (int i = 0; i < tileCountXY; i++)
		mapData[i] = 1;

	for (int i = 0; i < 20; i++)
	{
		int x = rand() % tileCountX;
	}

	sp = new iShortestPath();
	sp->set(mapData, tileCountX, tileCountY, 
		TileWidth, TileHeight);

	player = new Player;
	player->speed = 400;
}

void freePath()
{
	delete player;
	delete mapData;
	delete sp;
}

void drawPath(float dt)
{
	for (int j = 0; j < tileCountY; j++)
	{
		for (int i = 0; i < tileCountX; i++)
		{
			if (i % 2 == 0 && j % 2 == 0) setRGBA(1, 1, 1, 0.4);
			else setRGBA(1, 0.5, 0, 0.4);
			drawRect(TileWidth * i + 2, TileHeight * j + 2, TileWidth - 4, TileHeight - 4);
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
		int ix = ((int)p.x / TileWidth);
		int iy = ((int)p.y / TileHeight);
		int idx = iy * tileCountX + ix;

		if (mapData[idx] == I || ix > tileCountX - 1 || iy > tileCountY - 1)
			break;

		sp->run(player->currPoint, p, player->path, player->pathNum);
		player->pathIdx = 0;

		break;
	}

	case iKeyStateMoved:

		break;

	case iKeyStateEnded:

		break;
	}

	return false;
}

