#pragma once

#include "iStd.h"

void loadSettingUI();
void freeSettingUI();
void drawSettingUI(float dt);
bool keySettingUI(iKeyState state, iPoint p);
void showSettingUI(bool isShow);
