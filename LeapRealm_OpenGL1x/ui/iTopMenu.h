#pragma once

#include "iStd.h"

extern iPopup* topMenuUI;
extern iImage** imgTopMenuBtn;

void loadTopMenu();
void freeTopMenu();
void drawTopMenu(float dt);
bool keyTopMenu(iKeyState state, iPoint p);
void showTopMenu(bool isShow);
