#include "iShortestPath.h"

iShortestPath::iShortestPath()
{
	cost = 0;
	costX = 0;
	costY = 0;
	tileW = 0;
	tileH = 0;

	n = nullptr;
	nNum = 0;

	p = nullptr;
}

iShortestPath::~iShortestPath()
{
	if (n)
	{
		delete n;
		delete p;
	}
}

void iShortestPath::set(uint8* cost, int costX, int costY, int tileW, int tileH)
{
	this->cost = cost;
	this->costX = costX;
	this->costY = costY;
	this->tileW = tileW;
	this->tileH = tileH;

	if (nNum < costX * costY)
	{
		if (n)
		{
			for (int i = 0; i < nNum; i++)
				delete n[i];
			delete n;

			delete p;
		}
		nNum = costX * costY;
		n = new Node * [nNum];
		for (int i = 0; i < nNum; i++)
			n[i] = new Node(nNum);

		p = new int[nNum];
	}
}

void iShortestPath::run(int start, int end, int* path, int& pathNum)
{
	int costXY = costX * costY;
	for (int i = 0; i < costXY; i++)
		n[i]->init();

	n[start]->value = 0;
	n[start]->path[0] = start;
	n[start]->pathNum = 1;

	while (true)
	{
		int min = I;
		int curr = -1;
		for (int i = 0; i < costXY; i++)
		{
			if (n[i]->value < min && n[i]->visit == false)
			{
				min = n[i]->value;
				curr = i;
			}
		}

		if (curr == -1)
		{
			// 모든 곳을 방문 했으면
			break;
		}

		n[curr]->visit = true;

		for (int j = 0; j < 4; j++)
		{
			int k;
			if (j == 0)			// left
			{
				if (curr % costX == 0) continue;
				k = curr - 1;
			}
			else if (j == 1)	// right
			{
				if (curr % costX == costX - 1) continue;
				k = curr + 1;
			}
			else if (j == 2)	// up
			{
				if (curr / costX == 0) continue;
				k = curr - costX;
			}
			else				// down
			{
				if (curr / costX == costY - 1) continue;
				k = curr + costX;
			}

			Node* dst = n[k];
			Node* src = n[curr];
			int v = min + cost[k];

			if (dst->visit == false && v < dst->value)
			{
				dst->value = v;
				memcpy(dst->path, src->path, sizeof(int) * src->pathNum);
				dst->path[src->pathNum] = k;
				dst->pathNum = src->pathNum + 1;
			}
		}
	}

	Node* node = n[end];
	memcpy(path, node->path, sizeof(int) * node->pathNum);
	pathNum = node->pathNum;
}

void iShortestPath::run(iPoint s, iPoint e, iPoint* path, int& pathNum)
{
	int x = s.x; x /= tileW;
	int y = s.y; y /= tileH;
	int start = costX * y + x;

	x = e.x; x /= tileW;
	y = e.y; y /= tileH;
	int end = costX * y + x;

	run(start, end, p, pathNum);

	for (int i = 1, j = pathNum - 1; i < j; i++)
	{
		path[i] = iPointMake(tileW * (p[i] % costX) + tileW / 2, 
							 tileH * (p[i] / costX) + tileH / 2);
	}
	path[0] = s;
	path[pathNum - 1] = e;
}
