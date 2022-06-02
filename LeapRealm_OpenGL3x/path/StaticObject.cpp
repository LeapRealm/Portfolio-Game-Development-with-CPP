#include "StaticObject.h"

#include "Path.h"

StaticObject::StaticObject()
{
	rect.origin = iPointZero;
	rect.size = iSizeZero;
	isOverlap = false;
}

void StaticObject::setPosByIndex(int x, int y)
{
	rect.origin = mapRect.origin + iPointMake(TileWidth * x, TileHeight * y);
}

void StaticObject::paint(float dt)
{
	setRGBA(1, 0, 1, 1);
	fillRect(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
	setRGBA(1, 1, 1, 1);
}
