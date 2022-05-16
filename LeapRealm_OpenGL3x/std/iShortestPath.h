#pragma once

#include "iCommon.h"

#define I 0xFF

struct Node
{
	Node(int num)
	{
		value = I;
		visit = false;

		path = new int[num];
		pathNum = 0;
	}

	virtual ~Node()
	{
		delete path;
	}

	void init()
	{
		value = I;
		visit = false;

		pathNum = 0;
	}

	int value;
	bool visit;

	int* path;
	int pathNum;
};

class iShortestPath
{
public:
	iShortestPath();
	virtual ~iShortestPath();

	void set(uint8* cost, int costX, int costY, int tileW, int tileH);
	void run(int start, int end, int* path, int& pathNum);
	void run(iPoint s, iPoint e, iPoint* path, int& pathNum);

public:
	Node** n;
	int nNum;

	uint8* cost;
	int costX, costY;
	int tileW, tileH;

	int* p;
};
