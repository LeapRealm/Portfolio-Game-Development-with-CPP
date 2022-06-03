#include "Path.h"

#include "iLogUI.h"
#include "iStd.h"
#include "Player.h"
#include "StaticObject.h"

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

static iShortestPath* sp;
static Player* player;

static iLinkedList* staticObjects;

static iSort* sort = nullptr;
static PathObject** po;

void deleteStaticObject(void* data)
{
	delete (StaticObject*)data;
}

void loadPath()
{
	mapRect = iRectMake(devSize.width / 2 - TileWidth, 
						devSize.height / 2 - (TileCountX * TileWidth / 2 - TileHeight), 
						TileCountX * TileWidth, TileCountY * TileHeight);

	sp = new iShortestPath();
	sp->set(mapData, TileCountX, TileCountY, TileWidth, TileHeight);

	staticObjects = new iLinkedList(deleteStaticObject);

	struct Tree
	{
		int x, y;
	};
	Tree tree[5] = 
	{
		{2, 11},
		{5, 11},
		{3,9},
		{12,3},
		{15, 4}
	};

	int cnt = sizeof(tree) / sizeof(Tree);
	for (int i = 0; i < cnt; i++)
	{
		int x = tree[i].x;
		int y = tree[i].y;

		StaticObject* so = new StaticObject();
		so->rect.size = iSizeMake(TileWidth * 3, TileHeight * 3);
		so->setPosByIndex(x, y);
		staticObjects->addObject(so);
		mapData[(y + 2) * TileCountX + x + 0] = I;
		mapData[(y + 2) * TileCountX + x + 1] = I;
	}

	player = new Player();
	player->speed = 200;
	player->currLocalPos = player->targetLocalPos = iPointMake(TileWidth / 2, TileHeight / 2);

	sort = new iSort();
	po = new PathObject * [100];
}

void freePath()
{
	delete sp;
	delete staticObjects;
	delete player;
	delete sort;
	delete po;
}

void drawPath(float dt)
{
	if (getActionKeyPressed(KEY_SPACE))
		changeMap();

	drawTile();
	drawPathLine();

	sort->init();

	for (int i = 0; i < staticObjects->count; i++)
	{
		StaticObject* so = (StaticObject*)staticObjects->getObjectByIndex(i);
		sort->add(i, so->rect.origin.y + so->rect.size.height);
		po[i] = so;
	}
	sort->add(staticObjects->count, player->getCurrMapPos().y);
	po[staticObjects->count] = player;

	sort->update();

	for(int i=0; i< sort->sdNum; i++)
		po[sort->getIndex(sort->sdNum-1-i)]->paint(dt);
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
