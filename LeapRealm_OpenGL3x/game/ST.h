#pragma once

#include "iCommon.h"
#include "iShadertoy.h"

void loadST();
void freeST();
void drawST(float dt);
void keyST(iKeyState state, iPoint p);

extern iSTInfo stInfoTest[2][5];
