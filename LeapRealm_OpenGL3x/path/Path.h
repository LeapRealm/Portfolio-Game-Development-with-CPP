#pragma once

#include "iCommon.h"

#define TileWidth  30
#define TileHeight 30

extern iRect mapRect;

void loadPath();
void freePath();
void drawPath(float dt);
bool keyPath(iKeyState state, iPoint p);
