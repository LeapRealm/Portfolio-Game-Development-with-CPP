#include "game.h"

#include "iItemUI.h"
#include "iSound.h"
#include "iUI.h"

void loadGame()
{
	loadAudio(ai, snd_max);
	audioVolume(1, 1, snd_eff_max);
	audioPlay(snd_bgm_main);

	loadUI();
}

void freeGame()
{
	freeUI();
}

void drawGame(float dt)
{
	drawUI(dt);
}

bool keyGame(iKeyState state, iPoint p)
{
	keyUI(state, p);

	if (state == iKeyStateEnded)
		resetFlags();

	return true;
}
