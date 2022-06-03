#include "Path.h"

#include "iLogUI.h"
#include "iStd.h"
#include "Player.h"
#include "Tree.h"

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

iRect mapRect;

static int mapIdx = 0;
static uint8* mapData = _mapData[mapIdx];

static iShortestPath* shortestPath;
static Player* player;

static iLinkedList* trees;

static iSort* sort;
static PathObject** pathObjects;

void deleteTree(void* data)
{
	delete (Tree*)data;
}

void loadPath()
{
	mapRect = iRectMake(devSize.width / 2 - TileWidth, 
						devSize.height / 2 - (TileCountX * TileWidth / 2 - TileHeight), 
						TileCountX * TileWidth, TileCountY * TileHeight);

	shortestPath = new iShortestPath();
	shortestPath->set(mapData, TileCountX, TileCountY, TileWidth, TileHeight);

	trees = new iLinkedList(deleteTree);

	struct TreePos
	{
		int x, y;
	};
	TreePos treePoses[5] = 
	{
		{2, 11},
		{5, 11},
		{3,9},
		{10,3},
		{13, 4}
	};

	int cnt = sizeof(treePoses) / sizeof(TreePos);
	for (int i = 0; i < cnt; i++)
	{
		int x = treePoses[i].x;
		int y = treePoses[i].y;

		Tree* tree = new Tree();
		tree->rect.size = iSizeMake(TileWidth * 3, TileHeight * 3);
		tree->setPosByIndex(x, y);
		trees->addObject(tree);
		mapData[(y + 2) * TileCountX + x + 0] = I;
		mapData[(y + 2) * TileCountX + x + 1] = I;
	}

	player = new Player();
	player->speed = 200;
	player->currLocalPos = player->targetLocalPos = iPointMake(TileWidth / 2, TileHeight / 2);

	sort = new iSort();
	pathObjects = new PathObject * [100];
}

void freePath()
{
	delete shortestPath;
	delete trees;
	delete player;
	delete sort;
	delete pathObjects;
}

void drawPath(float dt)
{
	if (getActionKeyPressed(KEY_SPACE))
		changeMap();

	drawTiles();
	drawPathLine();

	if (mapIdx == 0)
		drawPathObjects(dt);
	else
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

		shortestPath->run(player->currLocalPos, p - mapRect.origin, player->path, player->pathNum);
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

	shortestPath->set(mapData, TileCountX, TileCountY, TileWidth, TileHeight);

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

void drawTiles()
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

void drawPathObjects(float dt)
{
	sort->init();

	for (int i = 0; i < trees->count; i++)
	{
		Tree* tree = (Tree*)trees->getObjectByIndex(i);
		sort->add(i, tree->rect.origin.y + tree->rect.size.height);
		pathObjects[i] = tree;
	}
	sort->add(trees->count, player->getCurrWorldPos().y);
	pathObjects[trees->count] = player;

	sort->update();

	for (int i = 0; i < sort->sdNum; i++)
		pathObjects[sort->getIndex(sort->sdNum - 1 - i)]->paint(dt);
}
