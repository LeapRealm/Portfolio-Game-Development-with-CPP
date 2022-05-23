#include "game.h"

#include "Map.h"
#include "Loading.h"
#include "ST.h"

void loadGame()
{
	loadAudio(ai, snd_max);
	audioVolume(1, 1, snd_eff_max);
	audioPlay(snd_bgm_main);

#if 1
	loadMap();
	gs = iGameStateMap;

#else
	loadST();
	gs = iGameStateST;
#endif
}

void freeGame()
{
	switch (gs)
	{
	case iGameStateMenu: /* freeMenu(); */	break;
	case iGameStateMap:		freeMap();		break;
	case iGameStateST:		freeST();		break;
	}
}

void drawGame(float dt)
{
	switch (gs)
	{
	case iGameStateMenu: /* drawMenu(dt); */break;
	case iGameStateMap:		drawMap(dt);	break;
	case iGameStateST:		drawST(dt);		break;
	}

	drawLoading(dt);
}

bool keyGame(iKeyState state, iPoint p)
{
	switch (gs)
	{
	case iGameStateMenu: /* keyMenu(state, p); */	break;
	case iGameStateMap:		keyMap(state, p);		break;
	case iGameStateST:		keyST(state, p);		break;
	}

	return true;
}
