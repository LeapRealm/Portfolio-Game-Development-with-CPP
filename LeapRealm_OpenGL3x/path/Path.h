#pragma once

#include "iCommon.h"

#define TileCountX  16
#define TileCountY  16
#define TileCountXY (TileCountX * TileCountY)

#define TileWidth  30
#define TileHeight 30

extern iRect mapRect;

void loadPath();
void freePath();
void drawPath(float dt);
bool keyPath(iKeyState state, iPoint p);

void changeMap();
void drawTile();
void drawPathLine();
void reorderLayer();
