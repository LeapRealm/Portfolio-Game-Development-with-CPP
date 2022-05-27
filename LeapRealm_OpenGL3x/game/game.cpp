#include "game.h"

#include "iStd.h"

#include "Map.h"
#include "Loading.h"
#include "ST.h"

void loadGame()
{
	loadAudio(ai, snd_max);
	audioVolume(1, 1, snd_eff_max);
	audioPlay(snd_bgm_main);

	gs = iGameStateMap;

	switch (gs)
	{
	case iGameStateMap:		loadMap();		break;
	case iGameStateST:		loadST();		break;
	}
}

void freeGame()
{
	switch (gs)
	{
	case iGameStateMap:		freeMap();		break;
	case iGameStateST:		freeST();		break;
	}
}

void drawGame(float dt)
{
	switch (gs)
	{
	case iGameStateMap:		drawMap(dt);	break;
	case iGameStateST:		drawST(dt);		break;
	}

	drawLoading(dt);
}

bool keyGame(iKeyState state, iPoint p)
{
	switch (gs)
	{
	case iGameStateMap:		keyMap(state, p);		break;
	case iGameStateST:		keyST(state, p);		break;
	}

	return true;
}
