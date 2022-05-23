#pragma once

#include "iStd.h"

void loadLogUI();
void freeLogUI();
void drawLogUI(float dt);
bool keyLogUI(iKeyState state, iPoint p);

void showLogUI(bool isShow);

enum MsgAttr
{
	MsgAttrGeneral = 0,
	MsgAttrNotice,
	MsgAttrWarning,
};

void addLogMessage(MsgAttr ma, const char* szFormat, ...);
