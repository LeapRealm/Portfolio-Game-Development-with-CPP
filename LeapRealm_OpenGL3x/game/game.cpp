#include "game.h"

#include "Map.h"
#include "Loading.h"

void loadGame()
{
	loadAudio(ai, snd_max);
	audioVolume(1, 1, snd_eff_max);
	audioPlay(snd_bgm_main);

	loadMap();
	gs = iGameStateMap;
}

void freeGame()
{
	switch (gs)
	{
	case iGameStateMenu: /* freeMenu(); */	break;
	case iGameStateMap:		freeMap();		break;
	}
}

void drawGame(float dt)
{
	switch (gs)
	{
	case iGameStateMenu: /* drawMenu(); */	break;
	case iGameStateMap:		drawMap(dt);	break;
	}

	drawLoading(dt);
}

bool keyGame(iKeyState state, iPoint p)
{
	switch (gs)
	{
	case iGameStateMenu: /* keyMenu(state, p); */	break;
	case iGameStateMap:		keyMap(state, p);		break;
	}

	return true;
}
