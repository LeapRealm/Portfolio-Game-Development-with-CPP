#pragma once

struct SortData
{
	int index;
	int value;
};

class iSort
{
public:
	iSort();
	virtual ~iSort();

	void init();
	void add(int index, int value);
	void update();

	int getIndex(int index);
	int getValue(int index);

public:
	SortData* sd;
	int sdNum;
};

#define sdMax 300
 