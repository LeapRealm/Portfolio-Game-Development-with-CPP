#include "Tree.h"

#include "Path.h"

static Texture* treeTex;

Tree::Tree()
{
	rect.origin = iPointZero;
	rect.size = iSizeZero;

	if (treeTex == nullptr)
		treeTex = createTexture("assets/path/tree.png");
}

Tree::~Tree()
{
	if (treeTex != nullptr)
	{
		freeTexture(treeTex);
		treeTex = nullptr;
	}
}

void Tree::setPosByIndex(int x, int y)
{
	rect.origin = mapRect.origin + iPointMake(TileWidth * x, TileHeight * y);
}

void Tree::paint(float dt)
{
	setRGBA(1, 1, 1, 1);
	drawTexture(treeTex, rect.origin.x, rect.origin.y, rect.size.width / treeTex->width, rect.size.height / treeTex->height);
}
