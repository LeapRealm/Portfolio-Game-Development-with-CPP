#include "iSort.h"

iSort::iSort()
{
	sd = new SortData[sdMax];
	sdNum = 0;
}

iSort::~iSort()
{
	delete sd;
}

void iSort::init()
{
	sdNum = 0;
}

void iSort::add(int index, int value)
{
	sd[sdNum].index = index;
	sd[sdNum].value = value;

	sdNum++;
}

void iSort::update()
{
	// 버블소팅
	for (int j = sdNum - 1; j > -1; j--)
	{
		for (int i = 0; i < j; i++)
		{
			if (sd[i].value < sd[1 + i].value)
			{
				SortData tmp = sd[i];
				sd[i] = sd[1 + i];
				sd[1 + i] = tmp;
			}
		}
	}

	// 퀵소팅
	/////////
}
 
int iSort::getIndex(int index)
{
	return sd[index].index;
}

int iSort::getValue(int index)
{
	return sd[index].value;
}
