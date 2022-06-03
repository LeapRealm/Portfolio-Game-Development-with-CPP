#include "StaticObject.h"

#include "Path.h"

StaticObject::StaticObject()
{
	rect.origin = iPointZero;
	rect.size = iSizeZero;
	isOverlap = false;

	tex = createTexture("assets/path/tree.png");
}

StaticObject::~StaticObject()
{
	freeTexture(tex);
}

void StaticObject::setPosByIndex(int x, int y)
{
	rect.origin = mapRect.origin + iPointMake(TileWidth * x, TileHeight * y);
}

void StaticObject::paint(float dt)
{
	setRGBA(1, 1, 1, 1);
	drawTexture(tex, rect.origin.x, rect.origin.y, rect.size.width / tex->width, rect.size.height / tex->height);
}
