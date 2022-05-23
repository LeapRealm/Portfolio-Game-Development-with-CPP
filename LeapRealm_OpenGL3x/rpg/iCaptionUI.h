#pragma once

#include "iStd.h"

void loadCaptionUI();
void freeCaptionUI();
void drawCaptionUI(float dt);
void showCaptionUI(float dt, int itemIndex, iPoint p);

extern iPopup* popCaptionUI;
