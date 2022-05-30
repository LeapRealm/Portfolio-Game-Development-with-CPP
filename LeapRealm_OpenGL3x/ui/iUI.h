#pragma once

#include "iStd.h"

typedef void (*LayerDraw)(float);
typedef bool (*LayerKey)(iKeyState, iPoint);
struct Layer
{
	LayerDraw draw;
	LayerKey key;
};

void loadUI();
void freeUI();
void drawUI(float dt);
bool keyUI(iKeyState state, iPoint p);

extern iLinkedList* listLayer;
extern Layer* inventoryLayer;
extern Layer* equipLayer;
extern Layer* creditLayer;