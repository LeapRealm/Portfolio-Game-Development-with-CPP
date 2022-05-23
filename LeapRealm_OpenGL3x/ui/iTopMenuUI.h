#pragma once

#include "iStd.h"

extern iPopup* topMenuUI;
extern iImage** imgTopMenuBtn;

void loadTopMenuUI();
void freeTopMenuUI();
void drawTopMenuUI(float dt);
bool keyTopMenuUI(iKeyState state, iPoint p);
void showTopMenuUI(bool isShow);
