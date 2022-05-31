#include "Map.h"

#include "iItemUI.h"
#include "iUI.h"
#include "Path.h"

Texture* texUIBg;

void loadMap()
{
	texUIBg = createTexture("assets/ui/scene.png");
	loadPath();
	loadUI();
}

void freeMap()
{
	freeUI();
	freePath();
	freeTexture(texUIBg);
}

void drawMap(float dt)
{
	drawTexture(texUIBg, 0, 0,
				devSize.width / texUIBg->width,
				devSize.height / texUIBg->height, TOP | LEFT);
	drawPath(dt);
	drawUI(dt);
}

void keyMap(iKeyState state, iPoint p)
{
	if (keyUI(state, p) == false)
		keyPath(state, p);

	if (state == iKeyStateEnded)
		resetFlags();
}
