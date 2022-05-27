#include "Map.h"

#include "iItemUI.h"
#include "iUI.h"

Texture* texUIBg;

void loadMap()
{
	texUIBg = createTexture("assets/ui/scene.png");

	loadUI();
}

void freeMap()
{
	freeUI();

	freeTexture(texUIBg);
}

void drawMap(float dt)
{
	drawTexture(texUIBg, 0, 0,
				devSize.width / texUIBg->width,
				devSize.height / texUIBg->height, TOP | LEFT);

	drawUI(dt);
}

void keyMap(iKeyState state, iPoint p)
{
	keyUI(state, p);

	if (state == iKeyStateEnded)
		resetFlags();
}
