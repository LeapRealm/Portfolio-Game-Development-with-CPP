#pragma once

#include "iStd.h"

void loadPauseUI();
void freePauseUI();
void drawPauseUI(float dt);
bool keyPauseUI(iKeyState state, iPoint p);
void showPauseUI(bool isShow);
