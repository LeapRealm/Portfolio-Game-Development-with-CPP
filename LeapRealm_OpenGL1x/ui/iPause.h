#pragma once

#include "iStd.h"

void loadPause();
void freePause();
void drawPause(float dt);
bool keyPause(iKeyState state, iPoint p);
void showPause(bool isShow);
