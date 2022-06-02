#include "Path.h"

#include "iLogUI.h"
#include "iStd.h"
#include "Player.h"
#include "StaticObject.h"

// TODO: 맵 교체 버튼

iLinkedList* drawLayer;
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

iLinkedList* staticObjects;

void deletePathObject(void* data)
{
	delete (PathObject*)data;
}

void loadPath()
{
	mapRect = iRectMake(devSize.width / 2 - TileWidth, 
						devSize.height / 2 - (TileCountX * TileWidth / 2 - TileHeight), 
						TileCountX * TileWidth, TileCountY * TileHeight);

	sp = new iShortestPath();
	sp->set(mapData, TileCountX, TileCountY, TileWidth, TileHeight);

	staticObjects = new iLinkedList();

	StaticObject* so = new StaticObject();
	so->rect.size = iSizeMake(TileWidth * 3, TileHeight * 3);
	so->setPosByIndex(4, 11);
	staticObjects->addObject(so);

	so = new StaticObject();
	so->rect.size = iSizeMake(TileWidth * 3, TileHeight * 3);
	so->setPosByIndex(9, 11);
	staticObjects->addObject(so);

	player = new Player();
	player->speed = 200;
	player->currLocalPos = player->targetLocalPos = iPointMake(TileWidth / 2, TileHeight / 2);

	drawLayer = new iLinkedList(deletePathObject);
	for (int i = 0; i < staticObjects->count; i++)
		drawLayer->addObject(staticObjects->getObjectByIndex(i));
	drawLayer->addObject(player);
}

void freePath()
{
	delete staticObjects;
	delete drawLayer;
	delete sp;
}

void drawPath(float dt)
{
	if (getActionKeyPressed(KEY_SPACE))
		changeMap();

	drawTile();
	drawPathLine();

	reorderLayer();
	for (int i = 0; i < drawLayer->count; i++)
	{
		PathObject* po = (PathObject*)drawLayer->getObjectByIndex(i);
		po->paint(dt);
	}
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

		sp->run(player->currLocalPos, p - mapRect.origin, player->path, player->pathNum);
		player->pathIdx = 0;

		break;
	}
	}
	return false;
}

void changeMap()
{
	mapIdx++;
	mapIdx %= 3;
	mapData = _mapData[mapIdx];

	sp->set(mapData, TileCountX, TileCountY, TileWidth, TileHeight);

	int ix = player->currLocalPos.x / TileWidth;
	int iy = player->currLocalPos.y / TileHeight;
	int idx = iy * TileCountX + ix;

	if (mapData[idx] == I)
	{
		addLogMessage(MsgAttrWarning, "벽이 존재하여 현재 위치가 초기화되었습니다.");
		player->currLocalPos = player->targetLocalPos = iPointMake(TileWidth / 2, TileHeight / 2);
	}
	else
	{
		player->targetLocalPos = player->currLocalPos;
	}

	player->pathIdx = 0;
	player->pathNum = 0;
	player->isDest = true;

	addLogMessage(MsgAttrGeneral, "맵을 변경하였습니다.");
}

void reorderLayer()
{
	for (int i = 0; i < staticObjects->count; i++)
	{
		StaticObject* so = (StaticObject*)staticObjects->getObjectByIndex(i);

		bool overlap;
		if (containPoint(player->getCurrMapPos(), so->rect))
			overlap = true;
		else
			overlap = false;

		if (so->isOverlap != overlap)
		{
			so->isOverlap = overlap;

			void* p = drawLayer->unlinkObject(player);

			if (so->isOverlap)
				drawLayer->addObject(0, p);
			else
				drawLayer->addObject(p);

			break;
		}
	}
}

void drawTile()
{
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
}

void drawPathLine()
{
	setRGBA(1, 1, 1, 1);
	setLineWidth(12);
	for (int i = 1; i < player->pathNum - 1; i++)
		drawLine(player->path[i] + mapRect.origin, player->path[i + 1] + mapRect.origin);
	setLineWidth(1);
}
