#pragma once

#include "iCommon.h"

#define TileWidth  32
#define TileHeight 32

extern int tileCountX, tileCountY, tileCountXY;
extern uint8* mapData;

void loadPath();
void freePath();
void drawPath(float dt);
bool keyPath(iKeyState state, iPoint p);
